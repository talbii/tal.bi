---
title: "Notes on `std::any` and RTTI in C++"
date: 2025-05-31T14:59:22+0300
tags: ["c++", "programming"]
---

Run time type information (RTTI) is a mechanism in programming languages which allows type
introspection at runtime. In C++, this is achieved using the `typeid` operator:

```c++
struct Base {
    virtual ~Base() = default;
};
struct Derived1 : Base {};
struct Derived2 : Base {};

auto main() -> int {
    std::unique_ptr<Base> p = std::make_unique<Derived1>();
    std::println("type of p = {}", typeid(*p).name()); // prints Derived1
}
```

This is neat at best, but not very useful outside some debugging. More useful, is that the object
returned by `typeid`, is of type `std::type_info`, and it also has an `operator==` implementation.

The usage of `dynamic_cast<>` requires RTTI as its implementation is quite straightforward:

```c++
template<typename To, typename From>
auto my_dynamic_cast(From* ptr) -> To* {
    if (typeid(*ptr) != typeid(To)) 
        return nullptr;

    return reinterpret_cast<From*>(ptr);
}
```

With one caveat (two if you care about template fluff like `std::remove_pointer_t` here and
`std::add_pointer_t` there) that `dynamic_cast<>` allows for "non-strict" casting too,
in the sense that this code won't work with the above implementation:

```c++
struct Derived3 : Derived1 {};

auto main() -> int {
    auto p = get_object();

    // won't work with my_dynamic_cast if p is Derived3 
    if (auto d = dynamic_cast<Derived1*>(p.get())) {
        // ...
    }
}
```

In order to actually implement this we'd need `std::type_info` to expose some kind of ordering
mechanism which respects base and derived classes.

There is `std::type_info::before` which almost provides this functionality, however:

1. The ordering is implementation defined and can change between program invocations
2. Is too generic, for example `typeid(void).before(typeid(Derived1))` returns `true` which might
make one think that `void` is a base class of `Derived1` (it isn't).

Regardless, the implementation of `dynamic_cast<>` just requires the compiler to save a DAG in the
binary and iterate on it whenever needed, which is to say this task isn't impossible.

Moving on; The usage of `dynamic_cast<>` or `typeid` usually indicates a bad design, so you should
aim to avoid using it when possible.

## C++ RTTI details

The benefits of RTTI don't come for free, for there is some runtime cost in the sense that the
binary is larger, and it occupies more memory.

First, note that the compiler generates a `std::type_info` structure only for the types which could
have `typeid` called on them at runtime. This includes:

1. Objects accessed in a non-polymorphic way for which we called `typeid` on. For example:

```c++
struct MyType { /* ... */ };
const auto& type_info = typeid(MyType); // generates std::type_info for MyType only
```

2. Objects with polymorphic types which we might have called `typeid` on. For example:

```c++
std::unique_ptr<Base> p = get_object();
const auto& type_info = typeid(*p); // generates std::type_info for Base, Derived1, Derived2, etc.
```

To be exact, the `std::type_info` is generated for each type which is instantiated somewhere in the
 code. Which specifically just corresponds to whether the compiler has generated a virtual table
(vtable) for the type. No vtable means no pointer to the type information, so no need to generate
one.

### So, how is `typeid` implemented?

There's no magic. The `const std::type_info&` returned by `typeid` can be equivalently seen as a
call to a virtual getter method which returns a statically allocated structure. The compiler adds
some fancy syntax around and automatically creates the type information for us, but using `typeid`
is just equal to:

```c++
struct MyRtti {
    struct RttiData {
        const char* name;
        // more fields...
    };

    virtual ~MyRtti() = default;
    virtual auto get_rtti() const -> const RttiData& = 0;
};

struct Base : MyRtti {
    const RttiData& get_rtti() const override {
        static MyRtti::RttiData info{ "Base" };
        return info;
    }
};
```

The compiler just saves all this hassle above from you.

In practice, however, the implementation above is optimized a bit by the compiler. Note that using a
virtual getter method which just returns the pointer to the statically allocated type info is a
bit redundant. As the compiler, when it generates a vtable it can also just place in it a pointer to
the `std::type_info` directly.

Of course, you (the programmer) can't do this, but the compiler can. This saves on both an unneeded
indirection in the form of a function call, and on a virtual destructor which isn't really needed.

To summarize, instead of a virtual function, the first field in the vtable is just a pointer to
`std::type_info`. Therefore, when you call `typeid(obj)` the generated code is roughly:

```c++
const void** v = obj->vtable;
return *static_cast<const std::type_info*>(v[0]);
```

## About `std::any`

`std::any` is a type-safe utility container class which can hold a pointer to any constructible
type. Essentially, it is the C++ version of using `void*` in C. Here's an example:

```c++
std::any a;               // empty
a = "hello world"s;       // now holds std::string
a = std::vector{1, 2, 3}; // now holds std::vector, freed string
a = 5;                    // now holds int, freed vector
```

Implementing support just for the code above isn't impressive, and the strength of `std::any` comes
with `std::any_cast<>` which gives the user a type safe way to cast between `std::any` and a
concrete type:

```c++
if (auto vp = std::any_cast<std::vector<int>>(&a)) {
    auto& vec = *vp;
    // use vec as usual
}
```

if `a` doesn't contain a `std::vector<int>`, then the cast will return `nullptr` (or throw an
exception if you use the reference-accepting version of `std::any_cast<>`).

Here's a peculiarity: if you look at the [documentation for GCC](https://gcc.gnu.org/onlinedocs/gcc-15.1.0/gcc/C_002b_002b-Dialect-Options.html#index-fno-rtti),
you don't see any mention of
`std::any` under the section of `-fno-rtti`:

> `-fno-rtti` <br>
> Disable generation of information about every class with virtual functions
> for use by the C++ run-time type identification features (`dynamic_cast` and
> `typeid`).

So... how does `std::any_cast<>` work, without RTTI? Well, it depends on how `std::any` is
implemented. I have checked in the `std::any` proposal, [N3804](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3804.html)
(Any library proposal, 3rd revision) and could not find any reference for the case when RTTI is
disabled (which makes sense, as this is a compiler-level feature and does not concern the language).

As far as one might be concerned, both libstdc++ (GNU's implementation) and libcxx (LLVM's
implementation) deal with the case where RTTI is disabled. To do so, both libraries first define a
fallback "type info" which maps a type to a unique address in memory:

```c++
template<typename T>
struct unique_address_helper {
    static constexpr inline int id = 1;
}

template<typename T>
using const void* UniqueAddress = static_cast<const void*>(
    &unique_address_helper<T>::id
);
```

To use `UniqueAddress`, at assignment to `std::any` you can also save in the class a pointer to
`UniqueAddress<T>` where `T` is the type just assigned. In `std::any_cast<U>`, just compare the
address saved with `UniqueAddress<U>`.

Here's how that approach looks in libstdc++. It checks for addresses and as a fallback compares
`typeid`'s if RTTI is enabled:

```c++
template<typename _Tp>
void* __any_caster(const any* __any)
{
    using _Up = remove_cv_t<_Tp>;
    // ...

    // First, try using the fallback for comparison.
    // _Manager is a class, equivalent to unique_address_helper, and ::_S_manage is a static 
    // function inside it, equivalent to ::id
    else if (__any->_M_manager == &any::_Manager<_Up>::_S_manage
    // in case RTTI isn't disabled, also use typeid
#if __cpp_rtti
             || __any->type() == typeid(_Tp)
#endif
      )
    {
        // check matches, perform cast
        return any::_Manager<_Up>::_S_access(__any->_M_storage);
    }

    return nullptr;
}
```

comments mine. Code is from libstdc++ and is available [here](https://github.com/gcc-mirror/gcc/blob/3d287bff14885598c75c1cb16b08e0ba4ba05bce/libstdc%2B%2B-v3/include/std/any#L523). For completion, the libcxx implementation
is available [here](https://github.com/llvm/llvm-project/blob/main/libcxx/include/any#L575). In
short, the implementation is similar to libstdc++ but instead of comparing static functions libcxx
uses an approach like the `unique_address_helper` defined above.
