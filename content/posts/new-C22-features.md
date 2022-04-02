---
title: "New C22 Features"
date: 2022-04-01
author: talbi
tags: ["c", "programming"]
---

<!--more-->

Recently, the new C22 standard draft was released, and with it some interesting features. This article will briefly discuss and showcase the new features added.

Per the draft, GNU has already began implementing some of C22's features and you can try them right now using the `-D__ENABLEC22_GNU` flag.

## References

C22 adds references, a feature that has been standard in C++ for many years. Now, you can use references in C as well. The syntax is similar to C++'s, using `TYPE& var`. Until compilers begin implementing C22 completely, you might have more luck using the `ref` keyword:

```c
void swap(int& a, int& b); /*C22 projected*/
void swap(int ref a, int ref b); /*partial implementation*/

/*Calling:*/
swap(a, b); /*C22 projected*/
swap(get_ref(a), get_ref(b)); /*partial implementation*/
```

You may check a working example [here](https://github.com/talbii/tal.bi/blob/blog/content/posts/new-C22-features/reference.c). Note, you need to download both the source file and the included header file(s).

## Function Annotation

Another exciting feature is function annotations, which actually perform static checking.

For example, similar to Java's `@NonNull` annotation, we can now use:

```c++
int do_stuff(@NonZero size_t n, @NonNull const int *array);
```

so, calling `do_stuff(0, NULL)` will result in a compilation error.

## Two-Variable Assertion

C22 adds a new macro: `assert2(MSG, COND)` which allows proper asserting with a message. For reference, today one would use the comma operator to do this: `assert("testing", i > 0)`, however the message would not be printed on failure. In contrast, `assert2` will print message when assertion fails, which is an improvement as it reduces abuse of the comma operator.

An example of using `assert2`:

```c++
int main(void) {
    int x;

    printf("Enter a positive integer: ");
    if(!scanf("%d", &x)) /*handle error*/

    assert2("x should be positive", x > 0);

    /*...*/
}
```

## Looping through `__VA_ARGS__` (va-loop)

When writing macros, one may use `...` to represent an unspecified number of arguments accepted. This is especially useful in writing macros, such as `errprintf`:

```c++
#define errprintf(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)
```

however, until C22 it was not possible to interact with `__VA_ARGS__`. C22 adds `va-loop`, which loops through `__VA_ARGS__` during compile time and allows basic manipulation:

```c++
#define errprintf(fmt, ...) va_loop()
#if va_current == MagicValue
#error "Stop! Recieved magic value."
#endif
fprintf(stderr, fmt, va_append()) va_endloop() 
```

## Classes

The main feature C22 adds is `class`es. It also removes any `structs`, so compiling with `-std=c22` will throw errors when declaring structures, instead of classes. However, the standard also hinted at *keeping* `struct` compatability, to support older programs.

The syntax to create a new `class` follows:

```c++
#include <class.h>

class my_class {
BEGIN_PRIVATE
    int x;
    int y; /*private fields*/
END_REGION
BEGIN_PUBLIC
DECLARE_CONSTRUCTOR (my_class) (int x, int y) 
    INITIALIZE_LIST
        _This->x = x;
        _This->y = y;
    END_ILIST {}

METHOD (int add(void)) {
    return _This->x + _This->y;
END_REGION
};
```

that was a lot of new syntax, so I'll try to explain everything that is happening here:

- First, to get the macros `BEGIN_PRIVATE`, `END_REGION`, `DECLARE_CONSTRUCTOR`, etc. we must include `class.h`, a C22 new header which defines the necessery macros to enable classes.
- Decleration of fields is straight-forward and is similar to how `struct`s do it.
- Declaring a constructor, which is a function with associated name and return type, we use the `DECLARE_CONSTRUCTOR(.)` macro which deals with that. After which we can input our constructor arguments and implement it.
- Similar to C++, we have an `INITIALIZE_LIST` macro which declares the variables we initialize during construction (that is, `my_class(...) : x(x), y(y) {}`.
- The `_This` pointer is a reserved keyword in C (which is why you **must not** rename your variables starting with underscores, it is in the C standard and has been for a long time) and, like other languages associates the current object with a pointer.
- Finally, declaring a method is as simple as the rest, using a `METHOD` macro at the beginning.

There is more to classes which I have not been able to showcase in the simple example above, including:

- Like `struct`s, `class`es are not `typedef`-d by default.
- C allows multiple inheritance
- There are no virtual functions. If one wishes to implement classes requiring virtual function, they should use `get_type` which returns the type of the variable during runtime.
- There are no abstract classes, nor interfaces (the C22 standard lists these as useless)

## Removal of Unused types (`short`, `byte`, `signed`, `char`, etc.)

C22 removes support of the types `short`, `byte`, `signed`, and more. These types are mostly (practically) unused, and so the standard has decided to drop them.

It is apparent that C22 wishes to remove as much keywords as possible, to allow for more room in the future (if needed). Still, I am not sure about the removal of the `signed` type, as I use it almost exclusively.

## Removal of `malloc` and similar functions

C22 also decides to remove `malloc` and similar functions (`realloc`, `calloc`, etc.). It does not add the `new` operator like C++. However, C22 does not remove the `free` function. The rational for this is simple - programs written before C22 have used `malloc` once or twice, and in order to avoid memory leaks C22 keeps `free` a valid function such that you can free the memory allocated before it. 

One might think: 'how come will I be able to implement some things which require `malloc`?' (e.g. buffers for error messages) - well, C22 recommends using the VLA ([Variable Length Arrays](/posts/vlas-in-c)) feature for things like that.

