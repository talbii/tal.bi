---
author: talbi
date: 2023-02-18T00:00:00+02:00
title: "[C++] is Turing Complete"
tags: ["cpp", "compilers", "turing"]
math: true
favorite: true
---

<!--more-->

A (complete) C++ compiler is Turing complete. Yes, the compiler, not the language (well, also the language but that is not very interesting). This means that, given enough time, one can run an entire program within compile time. Of course, this is not necessarily the case, as most modern compilers cannot run forever; they are still human made, thus containing errors.

But, *how is* a [compiler]() *Turing complete*?! What does that even mean? Moreover, what makes something Turing complete?

## Turing Completeness

> **Definition.** A *system* is called *Turing complete* iff it can simulate a Turing machine.

In layman terms, a system (or, in our case, a programming language [or its compiler]) is called Turing complete if we can simulate a Turing machine. That, of course, being a deterministic finite-state machine, with access to an infinite, random access, memory.

*But wait, infinite? I don't think my computer has infinite memory.* Well, in reality, a language is Turing complete, if it can run on any system with an arbitrarily-sized memory. For example, C is such language, as its standard is not concerned with physical limits (such as memory size). So, given a computer with, for example, 100 terabytes of memory, a C program can (and will) run on it with no problem.

In fact, a CPU *is* a Turing machine, albeit with finite memory, and [a lot]() (> 100 billion) of states. 

### Simulating a Turing Machine

When simulating a Turing machine, we essentially only have to worry about 3 things:

1. Read a cell
2. Move left from a cell
3. Move right from a cell

**Important:** we must support operations 2, 3 *conditionally*, i.e., an operation such as moving **if** the current cell's value matches something.

Here, we call a 'cell' a single bit in our infinite memory. Later, we will instead use a cell as an integer, simply for the sake of comfort, however a minimally working Turing machine can use as little as a single bit/cell. 

*Is this random access?* Previously, we said that a Turing machine needs to be able to randomly access a cell. However, if we are accessing some cell, then it must be in finite distance from our previous access, and so this access can be created as a composition of left/right moves. So, we are actually emulating a minimally working machine.

## C++ Compilers vs. Turing Machines

When I say that a C++ compiler is Turing complete, I actually mean that, a C++ compiler is able to simulate a Turing machine during the compilation process. There are two ways to do this: using `constexpr` methods, or, perhaps more interesting, by **parsing**.

Yes, parsing C++ requires a Turing machine, no less. Formally, if we define a language $\mathcal L$ as:

$$
\mathcal L = \left \lbrace P : P \text{ is a valid C++ program} \right\rbrace
$$ 

where $P$ is the source code of the program (e.g., $P = \texttt{import std;}$). Then, the language $\mathcal L$ is **recursive**, which is a class of formal languages which require a Turing machine in order to check if some string $s$ is in that language or not. This is equivalent to deciding whether a C++ program has any syntax error. But, *how* do we simulate a Turing machine using only syntax?

## Turing Completeness of C++ Syntax

In order to simulate a Turing machine, we use probably the best feature in C++: [templates]()! 

Recall, that some expression like `my_class<T>::method()` **must** resolve to *something*, at compile time. This means that the compiler will do anything possible (assuming the expression is valid) in order to find what it should place there. We can utilize this to achieve our goal.

While it is possible to just simulate a Turing machine, as presented in [this](https://citeseerx.ist.psu.edu/document?repid=rep1&type=pdf&doi=55a1417c034899636e736cfb168071555641dece) paper, we will do something else: we will run a *different* language, which by itself is Turing complete.

The language I chose to emulate is (*drum roll*): [Conway's FRACTRAN]()!

## FRACTRAN

FRACTRAN is an esoteric programming language, invented by the mathematician [John Conway](). The language itself is quite simple: a program is a list of fractions. The language works by finding the first fraction in that list that, when multiplied by the input $n$, results in some integer $n'$. After that, $n$ is set to $n'$ and this process is repeated.

The implications of this are massive. If we think of a number $n$ as its *prime decomposition*, for example, $24 = 2^3 \cdot 3^1$, then we can treat the multiplicity of some prime as a register. In the example of $24$ we have the register $r_2 = 3$ and $r_3 = 1$.

Armed with this knowledge, we can start to construct programs. For example, the following programs adds two integers $a + b$, where the input is of the format $2^a 3^b$. The output is placed in the register $r_2$ (so, the output is $2^{a+b}$):

$$
\left( \frac{2}{3} \right)
$$

this program works by repeatedly dividing the input by $3$, stopping only when we cannot do that. On each division, we also multiply by $2$. The effect of this is that on each iteration we are performing the transformation $2^x 3^y \to 2^{x+1} 3^{y-1}$.

As it turns out, FRACTRAN is [Turing complete](), which means that theoretically, we can simulate any Turing machine using it. Moreover, it is very easy to simulate FRACTRAN using C++ templates! It is important to emulate FRACTRAN using **templates**, as we want to show that the *syntax* requires Turing machine, which in turns means that we need to simulate FRACTRAN using types.


## Emulating FRACTRAN

Let us look at an example of our goal: let `P` be some type which corresponds to a FRACTRAN program, and `V<in, P>` a type which holds inside of it the execution of `P` under the input `in`. In order to achieve this, we use two features: template "recursion", and `enum`s. Perhaps more importantly, the value of some `enum` field must be evaluated at compile time. So, using recursion, we can get exactly what we want!

First, let us define a program. To do this, we use a "linked list", template-style:

```c++
template<int x, int A, int B, typename next_fraction>
struct fraction {
    enum { value = /* expression with input x */ };
};
```
where `next_fraction` is either a `fraction`, or `end_list`, which is used to denote the end of our input. This can be formalized using a `concept`, but not required here.

Implementation wise, inside `fraction` we *must* use an inner-class, such as `value_struct`. Because if we are inside `fraction`, then we have no way of directly calling `next_fraction`, unless we use `value_struct`. So, we now have:


```c++
struct end_list {   
    template<int>
    struct value_struct { enum { value = -1 }; };
    enum { value = -1 };

    // -1 is a placeholder value.
    // FRACTRAN does not deal with
    // negative integers, so this is OK.
};

template<int x, int A, int B, typename next_fraction>
struct fraction {
    template<int x_>
    struct value_struct {
        enum { value = /* expression with x_ */ };
    };

    enum { value = value_struct<x>::value };
};
```

We are left to define `fraction::value_struct<x_>::value`, and also a program type. The expression itself is simple: we try to multiply `x_` by the fraction represented by `A/B`, and on failure (i.e., when the result is not an integer) we try the next fraction in the list, as such:

```c++
/* snip */
enum { 
    value = ( (x_ * A) % B == 0 ) ? // check if fraction * x_ is an integer
                (x_ * A) / B :      // if so, this is our match
                next_fraction::value_struct<x_>::value // otherwise, continue iterating the list
};
/* snip */
```
and for the program type, we first need to remember that FRACTRAN does not halt until there are no fractions available. With that in mind, we have:
```c++
template<int input, template<int> typename fraction_list>
struct program {
    enum {
        output = (fraction_list<input>::value == -1) ? // an additional iteration will return -1?
                  input :                              // if so, `input` is our final output
                  program< fraction_list<input>::value,
                           fraction_list >::output     // otherwise, run another iteration
    };
};

// avoids infinite template-recursion:
template<typename T>
struct program<-1, T> { enum { output = -1 }; };
```
and with that code, we are done! This code is fully available [here](https://github.com/talbii/tal.bi/blob/blog/content/posts/cpp-syntax-tc/emulator.cpp).

In order to run a program, we first need to define the type `P`, which holds our program (the list of fractions), and an input. Afterwards, we apply those on the `program` type, which runs our program, and as a result `program::output` is the output.

In order to define `P`, we need a type which can receive a single integer as input, and output `::value`. To do so, we can use `using`:

```c++
// define the program
template<int input>
using adder_program = fraction<input, 2, 3, end_list>;
// (note that template declarations 
//  are forbidden inside functions)

constexpr auto input = 2 * 2 * 2  *  3 * 3;
```
and running:
```c++
template<int p>
int get_register(int x) {
    int count = 0;
    while ( x % p == 0) x /= p, ++count;
    return count;
}

int main(void) {
    auto output = program<input, adder_program>::output;

    fmt::print("3 + 2 is: {}\n", get_register<2>(output));
    // expected output: 5
}
```

### Side notes

Our emulator assumes several things:

1. `int` is sufficient: emulating FRACTRAN is especially difficult, as inputs can grow to extreme sizes. Our emulator uses `int`, without loss of generality. This `int` can be switched with a larger type, such as `long` or use [`_BitInt`](/posts/c23-bitint). Alas, all programs we write in C++ also use this assumption.
2. `fraction::value` can be avoided, albeit at a cost of uglier code.
