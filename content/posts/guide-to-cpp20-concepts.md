---
title: "An Introduction to C++ Concepts using Set Theory (kind-of)"
date: 2023-05-09
author: talbi
math: true
tags: ["cpp", "cpp20", "programming", "math"]
---

<!--more-->

<!--C++20 introduces a new feature called **Concepts**, which allow the programmer to define *concepts* (i.e., types) which have certain traits. In a sense, concepts are essentially Haskell's type classes, or Rust's traits. This is especially promising, as concepts allow the programmer to write much more scaleable code without loss of performance.-->

C++20 has been out for quite a while. With its introduction, several core features were added: three-way comparisons, modules, coroutines and more. One of these features is this article's main topic: **concepts**.

But what are concepts? And how can we utilise them in order to write better code?

<!--## C++'s Type System

C++ does not have a type system. It is a bit unique compared to other languages, in the sense that global functions, which are not entirely related to a type itself, can influence what we can do. For example, even though there is no `A::operator+` method, in the following snippet we are still able to use `+` on the type `A`:

```c++
struct A;
A operator+( const A &lhs, const auto &rhs);

int main(void) {
    A x, y;

    std::print("{} + {} = {}\n", x, y);
}
```-->

## Functionality

First and foremost, why do we need concepts? There are several reasons, but the main one which should motivate you is that they allow us to have **readable error messages**. C++ compilers are notorious for having long complicated error messages, even for just the simplest problem.

For example, printing a type which does not have an `operator<<` outputs over 100 lines in the error. Granted, compilers often include all candidates and that is not an actual error message, but still, C++ error messages could be much more readable.

<!--## So, what *are* concepts?

In *C++*, a **concept** is *like* a boolean variable, enabled by a type(s). -->

## Formalisations

In order to talk about C++ concepts, we briefly formalise them (like mathematicians do), and then apply that formalisation to our case.

> Let $U$ be some universe of objects. A *cater* is a set of objects $C \subseteq U$. A *cateroid* is the function $\phi_C : U \to \lbrace 0,1 \rbrace$ for which $\phi_C(o) = 1 \iff o \in C$.

To aid with notation, for some cateroid $f$, let us write from now $f(A) = \lbrace T \in A : f(T) = 1 \rbrace$.

Now, as with the case for boolean-valued functions, there are some operations we can perform on cateroids, including:
1. **Union/OR:** For cateroids $f, g$, let $h = f \cup g$ satisfy $h(U) = f(U) \cup g(U)$.
2. **Intersection/AND:** For cateroids $f, g$, let $h = f \cap g$ satisfy $h(U) = f(U) \cap g(U)$.
3. **Complementary/NOT:** For cateroid $f,$ let $h = \neg f$ satisfy $h(U) = f(U)^c = U \setminus f(U)$.

Now, by Boolean algebra, these three operations are enough to perform any operation we wish on cateroids.

---

Back to C++: In C++, a **concept** is *like* a boolean variable, which is enabled by a type, or a list of types. Continuing our analogy, these means that $U = \mathcal{P}(\mathsf{Type})$ in which $\mathsf{Type}$ is the set of all valid C++ types. <sup> [Nitpick](#ft:1) </sup>

Cateroids, are now exactly our concepts. In C++, a concept accepts a list of types, and 'outputs' a boolean value out, just like a cateroid. Not surprisingly, this also means that we can chain concepts together as we wish.

## Actual Code

We are now ready to see some actual code. A concept is defined by the `concept` keyword, similar to how a `using` is defined. A concept also must be preceded by a `template` declaration, defining the list of types our concept accepts.

```c++
template<typename T>
concept my_concept = /* ??? */
```

Now, to fill the `???`, we note that there can be one of two things in that place:
1. A `requires` expression, or:
2. A concept, or its negation, chained with another concept.

If we go back to cateroids, this can be explained inductively:

> A cateroid $f$ is called a *basis-cateroid* iff there are no cateroids $g, h \ne f$ for which $f = E(g, h)$, where $E$ is any expression involving $g, h$.

*Remark*:

> The set $\mathsf{Cateroids}(U)$ of all cateroids of $U$ can be defined inductively:
>   1. For each ${bc}$, a basis-cateroid, ${bc} \in \mathsf{Cateroids}(U)$.
>   2. Let $f, g \in \mathsf{Cateroids}(U)$, then $f \cup g, f \cap g, \neg f \in \mathsf{Cateroids}(U)$.

So, a `requires` expression would be analogous to a basis-cateroid. Inside such expression, we can input code which is required to be *valid for `T`*. This is checked by the compiler, during compile time. Failing to satisfy such requirement would fail that expression (i.e., set its value to `false`).

Here, for example, we require the type `T` to be **addable**, that is, have the function `auto operator+(T, T)`:

```c++
template<typename T>     /* for a, b of type T */
concept addable = requires (T a, T b) {
    a + b; /* this expression compiles */
};
```

Concepts can also check for specific methods or members:

```c++
template<typename T>
concept has_stuff = requires (T a) {
    a.f(); // has the method '<any> f(void)'
    a.x;   // has the field 'x'
};
```

Additionally, we may also constraint the types of some expressions:

```c++
template<typename T>
concept has_stuff2 = requires (T a) {
    /* compound ('group') an expression */
    { a.x } -> std::convertible_to<int>; /* built-in, <concepts> */
            /* constraints field 'a.x' to:
            (1) exist, and (2) be convertible to int */
    { 
};
```

Now, using only this functionality we can define pretty cool concepts. For example, here is a concept which checks for types which can be ordered (i.e., sorted):

```c++
template<typename T>
concept ord = requires (T a, T b) {
    { a < b } -> std::same_as<bool>;
};
```

## Using Concepts

In order to actually use the concepts we defined, we can use the `requires` keyword again, to require concepts onto a templated type. If the concept is unary (i.e., is templated by only one type), we can also use it as a templated type:

```c++
template<typename T> requires ord<T>
void my_sort(T *arr, size_t n);

template<ord T> /* equivalent */
void my_sort(T *arr, size_t n);
```

It is also possible to chain concepts following the `requires` keyword, however it should be avoided -- try to think whether this is the right approach, or should you define a concept which chains those concepts instead, and then use it.

```c++
template<typename T> requires C1<T> && (C2<T> || C3<T>)
void f();

// or:
template<typename T>
concept all_C = C1<T> && (C2<T> || C3<T>);

template<typename T> requires all_C<T>
void f();
```

## Readable Error Messages

To see the usefulness of concepts, let us look at the function:

```c++
template<typename T>
void print(T x) { std::cout << x << '\n'; }
```

and the struct

```c++
struct A {};
```

Previously, trying to run `print(A{})` would result in many lines of error. However, using the concept `printable`:

```c++
template<typename T>
concept printable = requires (T x) {
    std::cout << x;
};

template<printable T>
void print(T x) { std::cout << x << '\n'; }
```

we get the nice and concise message:

```
file.cpp:12:5: error: no matching function for call to 'print'
    print( A{} );
    ^~~~~
file.cpp:7:6: note: candidate template ignored: constraints not satisfied [with T = A]
void print(T x) { std::cout << x << '\n'; }
     ^
file.cpp:6:10: note: because 'A' does not satisfy 'printable'
template<printable T>
         ^
```

## To Summarize

Concepts allow us to define 'helper types' which are activated whenever some condition is met. These types, can be chained together using logical operations. Essentially, concepts give us two features:

1. Getting readable error messages
2. Writing code which is more generic, and semantically correct

Finally, I recommend also reading up on the built-in concepts, in the [`<concepts>`](https://en.cppreference.com/w/cpp/header/concepts) header.

---
<sup id="ft:1"> Nitpick </sup>
As seen, concepts can accept multiple types, **including** duplications. This means that something like `C<T, T>` *is valid*, and should compile (and indeed it does). In that case, how can $U = \mathcal{P}(\mathsf{Type})$, and not $\mathcal{MP}(\mathsf{Type})$ (multiset-powerset)?

Well, as we see later, concepts can be defined inductivelly. This means that we can define the concept `C<T, T>` as `C1<T> = C<T, T>`, thus omitting the need for multisets.
