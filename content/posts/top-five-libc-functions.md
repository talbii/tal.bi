---
author: talbi
title: "My 5 Favorite C Functions"
date: 2023-11-15T21:41:57+02:00
tags: ["c", "programming", "list"]
---

<!--more-->

In the style of "top 5", this post will go over five of my favorite C functions from the standard C, POSIX, GNU, and everything in between.

## `calloc` (standard C)

`calloc` is a function which is very similar to `malloc`. It allocates memory (normally, like `malloc`), however it also ensures that the returned memory is zeroed.

This can be useful in many scenarios, most notably in C-style strings, and a consequence of using `calloc` is avoiding a use-after-free bug. The returned pointer is compatible with regular `malloc` pointers, allowing resizing using `realloc` and freeing using `free`.

Where `calloc` differs from just a `malloc` and `memset` is in its call signature: `calloc(size_t num, size_t size)`. Thus, if in `malloc` one would use

```c
struct my_struct *p = malloc(n * sizeof *p);

if (p == NULL) 
    /* .. */
```

In `calloc` one would instead write:

```c
struct my_struct *p = calloc(n, sizeof *p);

/* .. */
```

While this difference is *very* subtle, the `calloc` version is actually safer. This is because the product `n * sizeof *p` might overflow, and cause less memory than expected to be allocated. `calloc`, on the other hand, does have a check for this overflow and if detected, no memory will be allocated. Maybe this is extremely unlikely to happen today, as `sizeof(size_t) == 8` on many computers, but this bug did happen, at least once: in 2002, for [OpenSSH](https://www.cvedetails.com/cve/CVE-2002-0639/)!

## `asprintf` (GNU, BSD)

String formatting is not a very neat task in C. Fortunately, the standard has `sprintf` (and later `snprintf`). Knowing in advance the required size for your buffer might not be possible, and while estimating its size is always possible, it can be bothersome and confusing.

Thankfully, GNU (and later BSD) have introduced `asprintf`: a `sprintf`-like function, which allocates the buffer for you!

The signature is straightforward: `int asprintf(char **ret, const char *fmt, ...)`. The function dynamically allocates `*ret`, depending on the size needed. Returned is the number of bytes written, which correlates to the size of the buffer allocated. After the caller is finished with the buffer, it can be freed with the regular `free()`.

## `getline` (POSIX)

While in the topic of automatic buffer allocation, `getline` also offers that. While `asprintf` *can* be seen as quite useless, as it is always possible to calculate a bound on the buffer size needed, it is not possible to do so with user input. Managing an increasing-when-full buffer can be a tedious task (although in C++ it is quite straightforward, via `std::vector<char_t>`).

The `getline` function does *everything* in one call:

1. Allocate, or resize a provided buffer
2. Read user input
3. Return the new (or old) buffer size

Here, for example, we can implement a basic, buffered `cat`-like utility, using few lines:

```c
FILE *fp = (argc > 1) ? fopen(argv[1]) : stdin;
if (!fp) /* .. */

char *buf = NULL;
size_t buf_size = 0;
ssize_t line_size = 0;

while ( (line_size = getline(&buf, &buf_size, fp)) > 0 )
    fwrite(buf, line_size, 1u, stdout);

free(buf);
```

## `arc4random_uniform` (BSD, macOS)

The `arc4random*` family of functions were introduced in several BSD distributions, including OpenBSD, FreeBSD, DragonFly, but also in the GNU C library. The purpose of these functions is to provide high-quality pseudorandom number generation.

When the well-known `rand()` and `random()` functions might use predictable, or simplistic pseudorandom number generators, `arc4random` aims to replace them by using a cryptographically-proven psuedorandom number generator. Its name stems from the (A)RC4 stream cipher, designed by Rivest. Since OS X 10.12 Sierra, the cipher was replaced with the AES cipher.

What's even more interesting, is the `arc4random_uniform` function, which uniformly returns a number in the interval `0, upper_bound`, with `upper_bound` specified by the caller. While the usual modulo trick will work, it also introduces a bias, called the *modulo bias*. If, for example, the maximal number which `arc4random` can return is 10, then taking the value of its call and applying modulo 3, will return the value of 2 with lower probability than that of 0 and 1 (`3/11` versus `4/11`). Hence, we introduced a bias.

Instead, the `arc4random_uniform` function not only uses the improved `arc4random` API, but also eliminates that bias, for the cost of potentially longer running time.

Moreover, the `arc4random*` family has another trick up its sleeve, which is "automatic" seed generation. The internal seed used is re-seeded on a regular basis, by using a kernel-provided random number.

## `fma` (standard C)

The `fma` function performs the **f**used **m**ultiply **a**dd operation, on floating-point numbers. Simply, `fma(x, y, z) = (x * y) + z`. I like this function is nice because many modern CPUs are able to perform it using a single *instruction*. For example, `fmadd` on ARM, or `vfmadd*` on x86 (FMA3/4 extension).

Unsurprisingly, `fma` is not the only function which has this optimization. One classical example, is `div`, which computes the integer division and returns both the division and modulo value. This function operates differently, relying on the fact that integer division in CPUs typically computes both values, so no additional cost is incurred by returning both values.

Another cool function is `sincos` (GCC, LLVM) or `__sincos` (Apple), which, as you might have guessed, computes both the sine and cosine of some floating-point number at the same time.
