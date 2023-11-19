---
title: "An Easy Factoring Problem (Factoring Prime Powers)"
date: 2023-10-21T23:23:46+03:00
math: true
tags: ["math", "number-theory"]
---

<!--more-->

Here is an easy factoring problem:

> **Problem:** Given a number $n = p^k \in \mathbb{N}$, where $p$ is prime and $k \in \mathbb{N}$, find $(p, k)$ in polynomial time w.r.t $\log n$.

In other words, we'd like to factor a prime power in polynomial time with respect to its representation in any base $b > 1$. Of course, the most trivial algorithm you can think of will work:

1. *For every $1 \le j \le \lceil \log_2 n \rceil$ do*:
2. Compute $x_j = \lfloor \sqrt[j]{n} \rfloor$.
3. *If ${x_j}^j = n$:* **Done:** $p = x$ and $k = j$.
4. *Else:* continue.

with the upper bound on $j$ being $\lceil \log_2 n \rceil$ as $p^{\log_2 n} = n ^ { \frac{1}{\log_p 2} }$ and because $\log_p 2 \le 1$ then $n ^ {\frac{1}{\log_p 2}} \ge n$.

Going a step back, we need to show several things first.

- **Is it polynomial?** How to compute $\lfloor \sqrt[j]{n} \rfloor$.
- **Is it correct?** Why is $\lfloor \sqrt[j]{n} \rfloor^j$ unique.

## Computing the Integer Root in Polynomial time

It is a good idea to start with a procedure for computing the *integer part* of an nth root. Denote with $\mathsf{iroot}(n, m) = \sqrt[m]{n}$ the integer $m$-th root of $n$, which is equal to the integer $y$ for which $y^m \le n^m < (y + 1)^m$. This is we want to compute. Moreover, let $f : \mathbb{N} \to \{ 0, 1 \}$ be the function:
$$ f(y) = \begin{cases}
1 & \text{if } y^m \le n \\\
0 & \text{else}
\end{cases}
$$

Now, notice that if $\mathsf{iroot}(n, m) = y^\prime$ then for any $y \le y^\prime$ we have $f(y) = 1$ and for $y > y^\prime$, $f(y) = 0$. In other words, $f$ is a monotonically decreasing function. This means that we can binary search on the interval $I = [1, n]$ for the largest (rightmost) number $y$ for which $f(y) = 1$. Binary searching on $I$ would take $\log n$ time, which is indeed polynomial.

## Correctness

Assume that for some $t$ we have ${x_t}^t = n$. The goal is to prove that $t = k$ and $x_j = p$. Notice that this is trivially correct, because of the *fundemental theorem in number theory*, which states that the prime factorization of any integer is unique. Thus, we have ${x_t}^t = p^k$ which must imply that $x_t = p$ and $t = k$.

On the other hand, assume there exist two such $t$s, $t_1$ and $t_2$, for which ${x_{t_1}}^{t_1} = {x_{t_2}}^{t_2} = n$. But again, this would imply that $x_{t_1} = x_{t_2} = p$ and ${t_1} = {t_2} = k$. This means that there exist **exactly one** index $1 \le t \le \lceil \log_2 n \rceil$ for which ${x_t}^t = n$.

---

This concludes the proof for the correctness of the algorithm. Its time complexity is $\log^2 n$, since for each of the $\lceil \log_2 n \rceil$ iterations, we compute $\mathsf{iroot}(n, j)$ once, which takes $\log_2 n$ time.

# An Improvement

While a time complexity of $\log^2 n$ is good, it is still technically quadratic hence has room for improvement. In fact, it is possible to improve the algorithm to run in $\log n \log \log n$ time.

Notice that on each iteration but the first, we perform unneccesary work. Given the value of $x_2 = \lfloor \sqrt{n} \rfloor$, it is obvious that $x_3 = \lfloor \sqrt[3]{n} \rfloor \le x_2$. This means that searching on the interval $I = [1, x_2]$ instead of $[1, n]$ would not impact the correctness, yet improve the running time of the binary search to $\log x_2 = \log \left( \lfloor \sqrt{n} \rfloor \right) = \frac{1}{2} \log n$ iterations.

Simiarily, we could do this for any subsequent iteration. Thus, when computing $x_{j + 1}$ it is possible to use $x_j$ as a bound, and compute $x_{j + 1}$ in $\log x_j = \frac{1}{j} \log n$ iterations.

Doing this for all iterations, yields a *total* running time of $\log n + \frac{1}{2} \log n + \ldots + \frac{1}{\log_2 n} \log n = \log n H_{\log_2 n} \sim \log n \log \log n$, where $H_m$ is the $m$th harmonic number.

