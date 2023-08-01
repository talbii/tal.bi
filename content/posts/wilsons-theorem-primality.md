---
author: talbi
title: "Primality Checking with Wilson's Theorem"
date: 2023-07-14T00:07:50+03:00
math: true
tags: ["math", "primes", "programming", "cpp", "optimizations"]
---

<!--more-->

The problem of *primality checking*, $\mathrm{PRIME}$, is perhaps one of the most famous and useful problems in computer science. The problem is described as deciding whether a given number $n \in \mathbb{N}$ is a prime number or not.

It was proved in 2004 that $\mathrm{PRIME} \in \mathsf{P}$. This means that there is some algorithm which checks whether a number $n$ is prime or not in $\mathcal{O}(\log^k n)$ time (where $\log n$ represents the size of the input, $n$ decoded in binary).

In this blog post, we will devise an **exponential** time (with respect to $\log n$) algorithm for primality checking, using Wilson's theorem!

## The Theorem

Wilson's algorithm can be stated in a short sentence:

> **Theorem** (*Wilson's theorem*) Let $n \in \mathbb{N}$. $n$ is *prime* **iff** $(n - 1)! \equiv -1 \pmod n$.

The important distinction here is the **iff**, which gives us a sure way to check for primality.

## The Algorithm

Perhaps not very surprising, but the algorithm will compute $(n - 1)! \bmod n$, and check whether it is equal to $-1$ (well, $n - 1$) or not. Here is a short implementation of it in C++:

```c++
#include <cstdint>

bool is_prime(std::uint64_t n) {
    std::uint64_t fact = 1u;
    for (std::uint64_t i = 2u; i < n; ++i)
        fact = (fact * i) % n;
    return fact == (n - 1);
}
```

Sure enough, this algorithm works wonders, although is a bit slow, taking 10.7 seconds to decide whether $2\\, 147\\, 483\\, 647 = 2^{31} - 1$ is prime or not (hint: it is).

## Sidetrack: Why is this slow?

Obviously, this algorithm is asymptotically slow, as it requires $n$ operations to decide whether $n$ is prime or not -- which, by definition, is exponential. However, modern computers can do about $10^9$ operations **per second** (ignoring many processor complexities such as branching, cache misses, etc.).

Taking the number $2\\, 147\\, 483\\, 647$ from before, assuming one operation for the loop, one for the multiplication and one for the modulo, we should expect the algorithm to run in about $\frac{3 \cdot 2^{31}}{10^9} \approx 6.4$ seconds, so what's going on here?

The answer here lies in our assumption of the modulo operation taking one CPU *operation*. In reality, this is completely wrong, and misleading. Typically, CPUs require **tens of cycles** in order to compute the integer division `x / y` for runtime values `x, y`. In turn, this means that computing the modulo is also expensive. In fact, in many modern CPUs computing either `x / y` or `x % y` yields the value of the other. You can get these values by using `div()` in `stdlib.h` (`std::div` in `cstdlib`).

Hence, even if the core loop itself is seemingly short, requiring only a few mathematical operations, in reality it could be very expensive. Also, both the loop and the multiplications are relatively efficient operations, so we are mainly bottlenecked by the modulo, as well as the number of iterations.

## Optimization 1: Faster divisions

[Libdivide](https://libdivide.com) is a fast and open-source library for computing *divisions*. This may sound silly, but in reality makes sense. In actuality, Libdivide shines when dividing a lot by a constant number (essentially, Libdivide does what the compiler does to compile-time values for run-time constant values).

In our case, we are not dividing, but rather taking the modulo, always with respect to `n`. However, notice that for integers `a, b` the division `a / b` is always integer (well, it is always floored), and that `(a / b) * b + (a % b) == a`. Hence, using a multiplication and a division it is possible to compute the modulo: `a % b = a - (a/b) * b`.

Implementing this is easy, just replace the inner-loop modulo with a Libdivide one:

```c++
bool libdivide_opt(std::uint64_t n) {
    std::uint64_t fact = 1u;
    auto divisor = libdivide::divider{n}; // new!

    for (auto i = 2u; i < n; ++i) {
        auto t = fact * i;
        fact = t - (t / divisor) * n;
    }
    return fact == (n - 1);
}
```

And the result? Running `libdivide_opt` with the number $2^{31} - 1$ (the same number from before) gives a result in just 9.25 seconds -- almost a 15% improvement!

## Optimization 2: Loop unrolling

The second 'cheap' optimization we can do is unroll the loop. Briefly, loop unrolling is the act of running multiple iterations of a loop in the same batch (i.e., without actually looping). For example, the loop:

```c++
for (auto i = 0; i < n; ++i) {
    f(i);
}
```
can be unrolled (unrolling of $2$) to:

```c++
auto i = 0;
//           vvv    vvv notice!
for (; i < n - 1; i += 2) {
    f(i);
    f(i + 1);
}

if (i == n - 1)
    f(i);
```
which is less understandable, but in practice the code above runs for *half as much* loops as before. 

The implementation of Wilson's algorithm is bounded by two things: the number of iterations, and the modulo operation per iteration. We already optimized the second bound, so how about the first?

Here is the final optimization of our algorithm:

```c++
bool libdivide_unrolled2(std::uint64_t n) {
    std::uint64_t fact = 1u;
    auto divisor = libdivide::divider{n};
    
    std::uint64_t i = 2u; 
    for (; i < n - 1; i += 2) {
        { // first unroll
            auto t = fact * i;
            fact = t - (t / divisor) * n;
        }
        { // second unroll
            auto t = fact * (i + 1);
            fact = t - (t / divisor) * n;
        }
    }

    if (i == (n - 1)) {
        auto t = fact * i;
        fact = t - (t / divisor) * n;
    }       

    return fact == (n - 1);
}
```

And the result?! A **decrease** in performance, when compared to the un-unrolled Libdivide version: about $9$ seconds compared to $8.7$. This teaches an important lesson: optimizations don't always work! At the end of the day, this algorithm is doomed from the get-go, and no clever optimizations will fix it.

---

## Benchmarking Notes

All benchmarks ran on an Apple M1 Pro processor. Below is the benchmarks output, `N` indicating the number of trials per function. Here are the functions I tested:

- `stock`: just the raw algorithm, no clever optimizations.
- `libdivide`: using Livdivide to compute moduli.
- `stock+unroll4`: the raw algorithm, but I've let Clang unroll the main loop with an unrolling factor of $4$.
- `libdivide+unroll4`: the Libdivide variant, but I've let Clang unroll the main loop with an unrolling factor of $4$.
- `libdivide+unroll42` the Libdivide variant, but I've manually unrolled the main loop with an unrolling factor of $2$.

```shell
Enter n: 2147483647
stock:
    Total: 51.4227s (N = 5)
  Average: 10.2845s
      Min: 10.2726s
      Max: 10.3014s

libdivide:
    Total: 43.663s (N = 5)
  Average: 8.73261s
      Min: 8.6733s
      Max: 8.90084s

stock+unroll4 (clang):
    Total: 50.0488s (N = 5)
  Average: 10.0098s
      Min: 10.0079s
      Max: 10.0122s

libdivide+unroll4 (clang):
    Total: 46.2684s (N = 5)
  Average: 9.25368s
      Min: 9.25102s
      Max: 9.25488s

libdivide+unroll2:
    Total: 45.3511s (N = 5)
  Average: 9.07022s
      Min: 9.06812s
      Max: 9.07358s
```

Code is available [here](wil.cpp). Code was complied using Clang, with the `-O2` flag. I used my library, [`bench.hpp`](https://github.com/talbii/bench.hpp) which performs several trials and averages the result. I used $N = 5$ trials, with the input $n = 2^{31} - 1$.

