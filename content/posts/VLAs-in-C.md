---
title: "VLAs: The C feature nobody uses"
date: 2022-03-16T21:07:31+00:00
author: talbi
tags: ["c", "programming"]
---

but why VLAs are unsued? and what are they?

<!--more-->


## What are VLAs?

VLAs, or *Variable Length Arrays* is a feature introduced in C99. In short, it allows the programmer to declare an array of an unknown size (of a *variable* size), as in the following code:

```c
void get_user_input(int n) {
    char buf[n];

    /*...*/
}
```

but wait, how? Well, as it turns out this is a mystery.

Actually, the compiler automatically deals with this, however the way it does so is left as an implementation note and is (probably) different between compilers. Essentially, one can think of this as automatic memory allocation and deletion. Well, this sounds great, right?

## Why VLAs are Hated

Well, as much as this sounds like a nice feature it has one major fault - the programmer does not know what is done behind the scenes. As Linus Torvalds [says](https://youtu.be/CYvJPra7Ebk), when you read C you know what the assembly output is, which is not something VLAs offer. From the example above arise many questions - how do we get the memory? what can we do with it? where is it allocated? what if memory acquisition fails?

In fact, Torvalds has [expressed](https://lkml.org/lkml/2018/3/7/621) his dislike in VLAs and as of Linux 4.2 (released August 2015) the kernel is free of VLAs.

## VLAs Today

Well, almost nobody uses them (or at least I hope so). In fact, many compilers just didn't care enough to implemenet VLAs that in C11 the macro `__STDC_NO_VLA__` was added to the spec, so C99 compatible compilers can say that they don't support VLAs specifically.

With that being said, most of the bigger compilers (such as gcc and clang) *do* support VLAs! It just happens that nobody wants to use them.

## In Summary

- Please, don't use VLAs!
- Many beginners use VLAs explicitly without knowing so (simply search for "C create array of size N"). I've seen too many use VLAs implicitly
- If you *do* happen to use VLAs (which you shouldn't!) understand their many quirks. Often `malloc` or even `alloca` is enough

