---
author: talbi
date: 2021-10-28
title: Fast $\varphi$ exponentiation
description: Calculate integer powers of the golden ratio using a trick
math: true
tags: ["golden-ratio", "fibonacci", "math", "programming"]
---

<!--more-->

The golden ratio has a very interesting property: for every power $n \in \mathbb{N}$, we can find a linear expression, such that $\varphi^n = A\varphi + B$ (where $A, B$ are constants). Let us prove this:

> *Lemma:* for all natural $n$: $\varphi^n = F_{n}\varphi + F_{n-1}$, where $F_k$ is the $k$th Fibonacci number

*Proof:* Using induction on $n$, and using the fact that $F_n = F_{n-1} + F_{n-2}$ and $\varphi^2 = \varphi + 1$.

After seeing this, one might be tempted to test the performance of using this method of calculation versus standard exponentiation - so I'll test that now. I will test 3 different techniques of exponentiation: the normal way ($x \cdot x \cdot \underbrace{\ldots}_{n - 3 \text{ times}} \cdot x$), the smart way ($x^\frac{n}{2} \cdot x^\frac{n}{2}$), and using the lemma we just proved. I'll be using C, to get the most performance out of our program.

## The Results

Below are the results of running the code ([GitHub](https://github.com/talbii/tal.bi/tree/blog/content/posts/fast-phi-powers/phi-powers.c)) on my machine (i7-8700k), in nanoseconds:

<details> <summary> Click to open table</summary>

$\varphi^n$ |`regular_power` | `smart_power` | `lemma_power`
------------|----------------|---------------|--------------
1|1100|1000|1100
2|1200|1100|1000
3|1100|1100|1100
4|1600|1000|1100
5|1100|1000|1000
6|1100|1000|1100
7|1100|1100|1000
8|1200|1100|1100
9|1000|1000|1000
10|1100|1100|1000
11|1100|1100|1100
12|1000|1200|1000
13|1100|1100|1100
14|1000|1000|1000
15|1000|1000|1100
16|1100|1000|1000
17|1200|1100|1000
18|1100|1000|1100
19|1100|1100|1000
20|1100|1100|1200
21|1300|1100|1200
22|1200|1000|1000
23|1200|1100|1100
24|1100|1100|1000
25|1100|1000|1200
26|1100|1100|1100
27|1100|1000|1100
28|1100|1100|1000
29|1100|1100|1100
30|1100|1000|1000
31|1100|1000|1000
32|1200|1000|1000
33|1200|1100|1100
34|1200|1100|1000
35|1100|1100|1100
36|1200|1100|1100
37|1100|1100|1100
38|1300|1000|1000
</details>

*compiled using `gcc -O0`, version `7.5.0`*

And a lovely graph:

![image: Graph of the results listed](graph.png)

Of course, these results should be taken with a grain of salt, as it is only one example on my machine and will vary between CPUs. But, overall our hypothesis has been confirmed - using the lemma does yield to faster calculations. How useful is this - is up to you. Note, that this also lines up with our expectation. For small $n$, using `lemma_power` is *slower* because of the overhead of calculating the Fibonacci numbers, but as $n$ gets larger this overhead becomes insignificant compared to the calculation itself.

I should also note that getting these results was a surprisingly difficult task, as it appears that multiplication of floating points on modern CPUs is pretty fast.

## Limitations

There are some limitations of this strategy. In real code you'd probably use `smart_power` if you are a good computer scientist, otherwise resort to `regular_power` which runs slower, but is easier to implement. The main limitation of `lemma_power` is of course the fact that you need to 1. calculate $n$ Fibonacci numbers, 2. Store them, and 3. know beforehand what is the maximal power (for running a sequence of calculations).
