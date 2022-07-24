---
author: talbi
date: 2022-07-23
title: Experiments with Allocators
tags: ["programming", "c++"]
---

<!--more-->

Recently I've been digging deeper into C++, which is the most complex language I think ever exists today (perhaps except COBOL). Today we will be looking briefly at `std::allocator`.

## What are Allocators?

In one line: allocators allocate and deallocate memory. -- Well that does not tell much. 

Briefly, an *Allocator* is another named requirement which satisfies things we've already known. For example, if `A`, `B` are different allocators (i.e., `A<T>`, `B<U>`) then `A::pointer` is convertible to `A::const_pointer`, and `A::void_pointer` and `B::void_pointer` are the same type.

Although this seems like a complication over just allocating (as many other things in C++) this is essentially just an abstraction to the concept of acquiring and freeing memory. This abstraction allows us to do some things, both useful and dumb.

## Modifying Allocators

As an example, we will write an allocator which logs its actions. Inheriting from `std::allocator<T>` does most of the job:

```c++
template<class T>
struct logging_allocator : std::allocator<T> {
    using super = std::allocator<T>;

    [[nodiscard]] constexpr T* allocate(std::size_t n) {
        auto *p = super::allocate(n);
        std::clog << "Allocated " << n << " bytes at " 
                  << reinterpret_cast<void*>(p) << '\n';
        return p;
    }

    constexpr void deallocate(T *p, std::size_t n) noexcept {
        std::clog << "Deallocating " << n << " bytes at "
                  << reinterpret_cast<void*>(p) << '\n';
        super::deallocate(p, n);
    }
};
```

and an example use case would be:

```c++
std::vector<int> vec(logging_allocator<int>{});
for(auto _ = 0; _ < 32; _++) vec.push_back(0);
...  
```

would output:

```shell
Allocated 1 bytes at 0x600001328030
Allocated 2 bytes at 0x600001328040
Deallocating 1 bytes at 0x600001328030
Allocated 4 bytes at 0x600001328030
Deallocating 2 bytes at 0x600001328040
Allocated 8 bytes at 0x60000112d100
Deallocating 4 bytes at 0x600001328030
Allocated 16 bytes at 0x60000042c1c0
Deallocating 8 bytes at 0x60000112d100
Allocated 32 bytes at 0x600003f2c080
Deallocating 16 bytes at 0x60000042c1c0
Deallocating 32 bytes at 0x600003f2c080   # destructor called
```

which indeed shows the dynamic resizing of `std::vector`.

## Having Fun

All the allocators mentioned in this post are available at my [GitHub](https://github.com/talbii/allocators). We will focus on one of them - `static_allocator<T, MAX_SIZE>`. As the name suggests, this allocator allows allocating up to `MAX_SIZE` bytes, and no more. Originally I've implemented it as a means to improve performance but unfortunately it turns out C++ is already pretty good at allocating memory (who would've guessed!).

Still, `static_allocator<T, MAX_SIZE>` *does* reach better performance (almost 2%!). Implementing is simple - we will only allocate:

```c++
template<class T, std::size_t MAX_SIZE>
struct static_allocator : std::allocator<T> {
    char *memory = new char[MAX_SIZE]; // too big to fit in stack
    std::size_t i = 0;

    [[nodiscard]] constexpr T* allocate(std::size_t n) {
        if(i + n >= MAX_SIZE) throw std::bad_alloc();
        return &memory[i += n * sizeof(T)];
    }
    ...
};
```

This allocator achieves almost a 2% improvement for a sequence of allocations and up to 50% worst case! Of course, the cost is that you must know in advance how much you will be allocating (which is useful in competitive programming, for example...).

