---
title: "Integer factorisation: what's the problem?"
date: 2022-12-10
author: talbi
tags: ["math", "complexity", "primes"]
math: true
---

<!--more-->

The problem of factoring is a very well studied problem, in which we know almost nothing. It is a good assumption that this problem, which we will refer to as $\mathrm{FACTOR}$ has been studied *longer* then the problem of just verifying primeness. Yet, even though this problem is well known, and relatively simple, we have yet to solve it.

But, what do I mean by "solve" $\mathrm{FACTOR}$?

## A little on complexity

> **Definition.** A problem $P$ can be solved in $T(n)$ time, *where $n$ is the **size** of the input* if there is some Turing machine $M$ which solves $P$ in less-or-equal to $c \cdot T(n)$ steps, where $c \in \mathbb{R}^+$.

In layman terms, our algorithm runs in $T(n)$ time if it does not take more than $T(n)$ steps to complete. The nitpick here, is how we get our input.

As with many computers, in the problem of factoring (and basically all problems in computer science), our input is the number $n$, *represented in binary*. This means that the size of our input is actually $\Theta(\log n)$, and not $n$ itself. As it turns out, this makes things much harder to solve.

## The $\mathrm{PRIME}$ problem

Before we look at $\mathrm{FACTOR}$, let us take a look at a simpler problem - deciding whether a given number is prime or not. In 2002, an article published by three Indian computer scientists (initials AKS) showed that indeed, given a number $n$, we can check if it is prime in $O(\log ^k n)$ time, for some $k > 0$. While the algorithm itself is quite complex, we will look at its simpler varient which achieves similar time, but is probabilistic.

The Miller-Rabin test checks for primality following a simple argument: if we randomly pick a number $y \in \mathbb{Z}^*_n$, then if $n$ is composite, there is a $\frac{3}{4}$-th chance that $y^{n-1} \not\equiv 1 \pmod n$ - and in that case, by Fermat's little theorem, we can conclude that $n$ is not prime. This test, while simple, achieves excellent running time in real-world computers, as there is little overhead for each pass.  The AKS algorithm, on the other hand, takes much longer due to edge-case treatment, and various overhead. Still, it was a revolution in computer science, and mathematics alike.

## The $\mathrm{FACTOR}$ problem

The $\mathrm{FACTOR}$ problem, is a simple problem where, given an integer $n$ we are asked to return a list of factors, such that $n = \prod f_i$. More formally, we are asked to return a list of primes, along with their multiplicity, such that $n = \prod {p_i}^{m_i}$. Note that this list of factors can be easily bounded by $\Theta(\log n)$, thus the main difficulty is actually finding the factors, rather than the number of different prime factors.

Today, it is not even known how hard $\mathrm{FACTOR}$ really is. This means, that we do not currently know whether $\mathrm{FACTOR} \in \mathsf{P}$, or even if it is $\mathsf{NP}$-complete. As a consequence, if it is not in either of those classes, then we'd have $\mathrm{FACTOR} \in \mathsf{NP}\text{-intermediate}$. Unfortunately, this would also not help, as problems such as the graph isomorphism, discrete log are also suspected to resign in that class.

In fact, there are some encryption protocols (e.g., RSA) that depend on the fact that integer factorisation is a hard problem. So, it seems unlikely that $\mathrm{FACTOR}$ would be in $\mathsf{P}$ - or, actually, there is great interest to crack this problem (yet it has not been cracked today).

## Current State of the Art

If we stick with the idea that $\mathrm{FACTOR} \notin \mathsf{P}$ (a decent assumption), but still wish to improve our runtime, then we are left with a new issue (besides finding a new algorithm): what time does it take? Our task is to find some function $t$ that satisfies
$$ O(\log ^k n) < t(n) < O(n^t) $$
for any $k \in \mathbb{N}, t \in \mathbb{R}^+$. Throughout day-to-day tasks, such functions $t$ are unheard of! Do they even exist? Well, they do, and this can be shown by taking $\log$ on all sides:
$$ k \log \log n < \log t(n) < t \log n $$
Inequality remains by the concavity of the $\log$ function. By letting $g(n) = \log t(n)$, we can see that $g(n)$ must be some logarithm, but to the power less-than $1$, and can also be $\log \log$, to the power of anything greater than $1$. In other words, we have:
$$ t(n) = \exp\left(c\cdot (\log n)^\alpha \cdot (\log \log n)^\beta\right) $$
for $\alpha < 1$ and $\beta > 0$.

As it turns out (shocker!) these functions do have a name, and are denoted using **$L$-notation**. Given some $n$, and $0 \le \alpha \le 1$, we have:
$$ L_n [\alpha, c] := \exp\left((c + o(1) \cdot (\log n)^\alpha (\log \log n)^{1-\alpha}\right) $$

Today, the best known deterministic algorithm is the [general number field sieve](https://en.wikipedia.org/wiki/General_number_field_sieve) (GNFS) algorithm, which runs in $\alpha = \frac{1}{3}$ time. Additionally, there is a probabilistic algorithm ([SSL](https://en.wikipedia.org/wiki/Integer_factorization#Schnorr%E2%80%93Seysen%E2%80%93Lenstra_Algorithm)) which in $\alpha = \frac{1}{2}$ time, albeit with a much smaller constant (half of that of GNFS, to be exact).

