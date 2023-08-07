---
author: talbi
title: "Beware when moving a `std::optional`!"
date: 2023-08-07T22:57:09+03:00
tags: ["programming", "cpp", "pitfalls"]
---

<!--more-->

This is a short post describing a pitfall when working with `std::optional`s.

Let's say that you have a `std::optional<T>` that you received from someone. If it exists, that's good, we want to return it and move on with the program. If it doesn't exist, however, then we terminate. In that case, we might have something like:

```c++
template<typename T>
T do_something() {
    std::optional<T> opt = some_oracle<T>();

    if (!opt) {
        std::cerr << "Something terrible happened\n";
        std::exit(EXIT_FAILURE);
    }

    return *opt; // equivalent to .value(), but doesn't throw an exception
}
```

Okay, `do_something()` is fine, however returning `*opt` requires a copy, since `std::optional<T>::operator*()` returns `T&`. Most often (and especially in our case), this is not something desirable, instead wasting time -- the value is destroyed when we exit `do_something()`.

Fine. The simple solution is to just move the value `*opt`, right? **NO!!**

While in the example above, that would work *just fine*, this is not something you should do. In reality, doing this leaves the optional value as an "intermediate", in-between, probably-not-desirable value. Here's an example:

```c++
std::optional<T> opt = some_good_oracle<T>(); /* assume opt.has_value() */

opt.has_value(); // true, expected

f1( std::move(opt.value()) ); // move the value, meaning that from 
                              // here onwards it is not needed

if (opt.has_value())  // true, unexpected!
    f2( std::move(opt.value()) ); // move to save time! 
                                  // in reality f2 got an empty/garbage T
```

It might seem obvious here, but I'm certain that this has bitten some developer sometime.

## The Solution?

The answer to all of your problems is very simple: move *the optional value*, instead of the value itself. If you wish to retrieve the actual value, just do that!

```c++
// BAD
// std::move(opt.value());

// Good
auto x = std::move(opt).value();
```


Moving the optional value works because the leftover variable, `opt`, will *not* have a value, thus it is not possible to accidentally access an empty/garbage value.

