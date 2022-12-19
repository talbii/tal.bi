---
title: "Arbitrary-sized Integers in C23"
author: talbi
date: 2022-12-19
tags: ["c", "programming" , "c23"]
math: true
---

<!--more-->

Since 2023 is getting close, and with it (hopefully) an implementation of the C23 spec, in this post I will showcase one of the exciting features coming to C programmers: `_BitInt`, which allow the programmer to define integers of **exactly** $N$ bits (in contrast to something like `int_least32_t`), where $N$ is given at compile time.

In comparison to C++, this is similar to `std::bitset`, but is actually quite different, in the sense that the programmer can interact with the integer using arithmetic operations (e.g., `+` or `*`) whereas `std::bitset` only allows bit manipulation.

## Rationale

The main reason behind this proposal, is to enable developers programming specific algorithms, or more generally applications where a specific size of variables is required - such as in cryptography, graphics (RGB values), IP addresses (or in general, embedded applications), to express themselves better.

For example, a function which accepts an RGB value of $K$ bits (where $K$ is known at compile time), can be written as:

```c++
constexpr size_t K = /* compile time value */;
typedef _BitInt(K) color;

struct RGB {
    color R;
    color G;
    color B;
}; /* sizeof(RGB) == 3 * K */
```

The second reason for adding this, is for developers targeting FPGAs, a type of embedded processor. In these systems, there is two main differences in comparison to traditional desktop CPUs:

1. **Memory is usually expensive:** so, replacing `int` (which often 32 or 64 bits) with `_BitInt(X)` is much more efficient, space wise.
2. **Word size is flexible:** since FPGAs are used in embedded applications, manufacturers implement integers of larger size, depending on the application. Thus, using integers of specific size may not impact performance, and actually benefit it. 

Finally, using `_BitInt` removes the headache of trying to write true portable code, as the size of types like `int` is not defined in the specification, but rather only defined to be at least *something* (e.g., `int` is at least 8 bits).

## State of Compilers

Currently (December 2022) the only compiler to implement this feature is Clang, and that is only due to an extension in LLVM, previously called `_ExtInt` which did exactly this. So, if one wants to test this feature, use Clang version 15 (or earlier versions, with the directive `#define _BitInt(X) _ExtInt(X)`).

It should be noted that the name `_ExtInt` is now depracted, in favour of the new `_BitInt` type.

## Some Fun

Since Clang already supports this feature, we can have some fun! Here for example, we allocate one mebibyte ($2^{20}$ bits), and checking whether $2^{2^{20}}$ is even or not (just to be sure):

```c++
#define _BitInt(X) _ExtInt(X)

int main(void) {
    _BitInt (1048576) x = 1 << (1 << 20);

    bool is_even = x % 2 == 0;
    if(is_even)
        puts("2^(2^20) is even!");
    else
        puts("2^(2^20) is odd!");
}
```

The result:

```text
2^(2^20) is even!
```

Safe to say my CPU is alright. It might be good to note that the resulting binary is 1.5M in size, which is to be expected (by the way, the assembly (ARM) was just short of 400,000 lines of code). So perhaps creating huge integers is not a great idea.

See also: [N2763](https://open-std.org/JTC1/SC22/WG14/www/docs/n2763.pdf), for the official specification.

