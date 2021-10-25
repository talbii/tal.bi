---
author: talbi
date: 2021-10-24
title: Exception handling in C
description: different ways to add "exceptions" to C
tags: ["c", "programming"]
---

3 ways to add exceptions to C

<!--more-->

---

Currently, C (C2x) doesn't feature exceptions - which makes sense - in order to add support for exceptions you would need some sort of an inheritance system, and also backend support (in the language itself) for exceptions, things that will make C much slower and less *C*, and more like C++.

Still, having some sort of an exceptions system can be a useful thing in programming. In this article I will showcase the 3 ways I have found to deal with this problem, their advantages and disadvantages.

## 1. Returning a Pointer

The most common way to add support for exceptions is by returning a *pointer*, instead of a normal variable. This way, in case of failure you can easy let the programmer receiving the result by returning `NULL`. This is done in a lot of the std C library - for example, `malloc`, `fopen`, `calloc` to name a few.

For example, if we have a function `add(int a, int b)` then you'd write something like:

```c
int* add(int a, int b) {
    int *res = malloc(sizeof res);
    if(!res) exit(1); /* not very good */
    
    if (
        ((a > 0) && (b > INT_MAX - a)) || 
        ((a < 0) && (b < INT_MIN - a)) 
        ) return NULL; 

    *res = a + b;

    return res;
}
```

... or something like that. In any case, this solution isn't great (*especially* in the above example).

- **Advantages:** very easy to implement, simple to understand and check in receiving code
- **Disadvantages:** can (and probably will) create ambiguous handling - a function may return `NULL` for different exceptions (for example, `malloc` returns `NULL` if there is not enough memory *or* `size` is zero). Also, adds more responsibility on the user to later `free()` the memory.

## 2. Returning an Integer

Another solution employed by std C is returning an integer representing some value known to the caller. For example, `scanf` returns the number of successful inputs, and `printf` returns the number of characters printed.

This solution is better than solution (1), as you don't have the ambiguous handling, but creates a new issue - if a function returns `0` on success, `1` on `Fail1` and `2` on `Fail3`, then handling this can be a bit weird, as `0, 1, 2` are all magic constant.

- **Advantages:** removes the issue of ambiguous handling.
- **Disadvantages:** creates the issue of dealing with magic constants.

## 3. (Global) Enum

This is the main solution deployed by C, and is the one I also like. Apart from returning a `NULL` on error, you would also set some variable to indicate error. In std C, this variable is called `errno` and is accessible from `<errno.h>`.

For example, we can write a function `read_file` like so:

```c
enum FileExceptions {
    Ok,
    PathDNE,
    FileReadErr,
    FileTooBig,
} fexcept;

FILE* read_file(char *path, char *mode);
```

and set `fexcept` accordingly.

- **Advantages:**
  - still relatively easy to implement, and allows for much more error types
  - checking against an error is a lot more understandable
  - easy to add more exceptions as code gets more complex
- **Disadvantages:**
  - Having a *global* exception makes it difficult to track multiple function calls
  - Having different exception types can get difficult to track in the main function (and checking for exceptions as well)

## 4. Local Variable Union + Enum

I believe, the best solution, is to use a *local* exception variable (and pass it by pointer). We use a `union` in order to make it possible to return different exception *types* from different functions, while keeping it all under the same type. Here is an example:

```c
union Exception {
    enum { Ok = 0, } Ok; /*The base "no exception" value*/
    enum {
        Overflow = 1, /*note we must begin with 1, otherwise Ok == Overflow. */
        Underflow,
        DivisionByZero,
        DataRace,
    } arithmetic_exception;

    enum {
        InvalidPath = 1,
        FileDoesNotExist,
        MissingPermissionsRead,
        MissingPermissionsWrite,
    } file_exception;

    enum {
        GotNULLPointer = 1,
        GotNegativeValue,
        GotNonTerminatingString,
        GotIndexOutOfRange,
    } invalid_data_exception;

    /*...*/
};
```

then, using this new exception system would be... easier:

```c
void f(..., union Exception *e); /* may throw arithmetic_exception */
void g(..., union Exception *e); /* may throw file_exception */

int caller() {
    union Exception e = {.Ok = Ok};

    f(..., &e);

    if(e.arithmetic_exception == DivisionByZero) {
        puts("Enter a non-zero value!");
    } else if(e.arithmetic_exception == Overflow) {
        puts("Enter a smaller value!");
    }

    g(..., &e);

    if(e.file_exception == FileDoesNotExist) {
        puts("Invalid path!");
    }
    
    /*...*/
}
```

- **Advantages:**
  - allows for "pseudo-inheritance" of exception handling
  - readable, no magic numbers
  - single type to deal with exceptions
  - different callers may have different local exceptions
- **Disadvantages:**
  - reduces readability by a lot, and in general is much longer to write
  - can cause bugs if one forgets to initialize `enum`s with `= 1`
  - may be redundant for functions that will only return one type of exception, and in general functions that only error out in few ways

While this solution is (probably) the best way to get OOP-like exceptions in C, it defiantly isn't the most convenient, and in my projects I'd usually use a combination of solutions (3) and (4). (create *local* exceptions, but without a union)

*Tip:* to write better code, I'd recommend splitting your project into sub-projects ("partitioning" it), and in each part deploy a different exception system. For example, if one part of your code deals with getting file input from the user, then for that singular part create a unified `enum FileException`, which (in the context of that single partition) will make sense.
