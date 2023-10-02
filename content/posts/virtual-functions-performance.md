---
title: "The Cost of Virtual Functions"
date: 2023-10-02T22:13:37+03:00
author: talbi
math: true
tags: ["cpp", "programming", "performance"]
---

<!--more-->

Virtual functions are a necessary feature in order to enable *polymorphism*, a very important programming paradigm (specifically, in object-oriented and functional). However, did you know that virtual functions also impose a performance hit? In fact, virtual functions can be up-to **twice as slow** as their non-virtual counterparts.

In order to understand why, and how this can be avoided, one has to understand how virtual functions, and methods are implemented.

## How Methods Work

Consider the regular method `f`:

```c++
struct A {
    int f(void) const;
};
```
and the following code, using it:
```c++
A x{};
auto result = x.f();
```

When compiling this code, compilers strip <sup> 1 </sup> the `struct`-ness of `A`, moving all of its methods into a namespace, with some name (e.g., the structure's name). In our case, the function `f` will be turned into the function `int f(const A*)`. Now, because all methods are just ordinary functions, they have a single address associated with them. The compiler can then replace any call to a method with a call to a function, which receives the instance as its first argument.

In general, the compiler keeps a table which it references whenever a call to some *non-virtual* method happens. The table has an entry for each function, specifying its location in memory, which is known at compile-time.

A call to a normal function has only one memory access: calling a function at a compile-time-known address.

<sup> 1 </sup>: This is not entirely accurate, however serves a good ground for this discussion. In C++ a structure (or class) is just syntactic aid, just like encapsulation is just a compile-time feature. At compilation, methods are moved to be global functions, in fact, are not "inside" the class). The structure itself is converted to a C-style structure.

## How Virtual Methods Work

Consider the virtual method `g`:

```c++
struct B {
    virtual int g(void) const;
};

struct C : public B {};
struct D : public B {};
```
and the following code, using it:
```c++
B *x = get_B(/*runtime value*/);
auto result = x->g();
```

In the initiation of `g` in the code above, the call may yield the result of running one of three different versions of `g` (originating from `B, C, D`). The exact function which will run is figured during runtime. If previously the compiler kept a table for each type, which specifies the address of all methods, in the case of virtual functions the compiler attaches a field to each instance, called the *virtual table*, which specifies addresses. Afterwards, calling a virtual method simply references the virtual table once, and then calls the function at the address specified inside of it.

Essentially, the compiler places the type's table inside the instance. Notice that this is **mandatory**, since implementing polymorphism requires some way of providing *different types* through a single interface. Failing to bookkeep some information would mean that it would not be possible to do this.

In contrast to normal functions, virtual functions require two memory accesses: one to access the virtual table, and another to call the function. This means that for functions that perform very little operations, calling the virtual variant would be twice as slow.

## Some Numbers

I ran the following snippet of code to measure the time it takes to dispatch a normal and a virtual function:

```c++
struct A {
    virtual ~A() = default;

    int f(void) const { return 3; }
    virtual int g(void) const { return 3; }
};

auto *x = new A;

x->f(); // test 1
x->g(); // test 2
```

The code ran for $N = 100 \\, 000 \\, 000$ trials. Note that each test ran independently of each other. In the case of interleaving (which can happen in actual code) the performance of virtual functions is even worse, because of cache locality. 

When running on an Apple M1 Pro, *on average* both functions took the same amount of time. However, in the worst case normal functions took `71 us`, whereas virtual functions took `136 us`, which indeed is almost twice as slow. Optimizations were disabled, to avoid function calls being removed for being unused.

In a different test I performed, where the member functions were not `const`, even with optimizations (`-O2`) the results were `47 us` (normal) versus `96 us` (virtual). It is evident that optimizations are not the issue, and virtual function do run slower than normal ones.

My test code is available [here](https://gist.github.com/talbii/5a5f2661ca2271d4812b24d26a6077f1).

## Well, I have enough cache!

Unfortunately, that is also not enough. In practice, *real world code* is large enough to impose cache locality issues. 

Even worse is the topic of optimizations. The simple fact is that virtual functions **cannot** be optimized away. This is because the compiler simply *cannot know* (in most cases) what function is going to be called. The example above might not demonstrate this, however having a base class and deriving it is enough to make the compiler not optimize any call at all.

Another issue is *inlining*. Again, virtual functions cannot be inlined, because the compiler doesn't know what to inline. With normal functions, everything is possible, and in fact a simple function (such as a setter, getter) can be almost optimized away into a single instruction. Virtual functions, no matter how simple, cannot benefit from this.

<!--Thus, in the case of a *const method*, whose value is unused, the compiler is free to completely optimize its call away. So, in the test above, test 1 runs in -->

## So... No to Virtual Functions?

Also no. Virtual functions are very important, for polymorphism, and also for functional programming. They allow for many things, including reducing clutter and complexity. Simply, focusing on eliminating virtual functions would be considered as a *premature optimization*. Moreover, you may already use virtual functions without noticing -- `std::function` is implemented with virtual functions, in order to support polymorphism at runtime.

Instead, you should focus on the bigger picture: use efficient data structures, and restructure code to run in the most efficient path. As a last resort, you can attempt to eliminate virtual functions, but note that micro-optimization is not so simple: branching, cache locality and memory layout are several other things that could also bite you.

One simple 'trick' is to replace virtual function calls with templated ones. Instead of having something like:

```c++
struct MyOperation {
    virtual int operator() (int, int) const = 0;
};

void do_something(const std::vector<int> &vec, const MyOperation *op);
```
which is a popular paradigm, consider using instead:
```c++
template<typename F>
void do_something(const std::vector<int> &vec, F &&f);
```

Moreover, C++20 has introduced [concepts](/posts/guide-to-cpp20-concepts). Using them, we can get full semantic correctness, *and* no performance hit:

```c++
template<typename F>
concept MyOperation = requires(F &&f) {
    { f( int{}, int{} ) } -> std::same_as<int>;
};

template<MyOperation Op>
void do_something(const std::vector<int> &vec, Op &&op);
```

<!--With this in mind, here's a simple way to gain a hefty boost in performance: sort by type! Assume you have one base class, and $100$ classes deriving it. Then by sorting a `std::vector<Base*>` by type (i.e., group classes of the same type together), you can improve performance highly: the same types call the same functions, so cache locality is highly improved.-->

