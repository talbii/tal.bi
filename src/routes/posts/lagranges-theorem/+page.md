---
title: "Lagrange's Theorem Done Quick"
description: "A complete proof of Lagrange's theorem (group theory), with as little as possible words, while remaining completely legible."
date: 2024-01-10T00:33:32+0200
tags: ['math', 'done-quick', 'group-theory']
---

Lagrange's theorem is a relatively simple and powerful theorem. In its core, it says that the order of any subgroup divides the order of the original group. In fact, Lagrange also specifies the exact value of the quotient.

> **Theorem.** (*Lagrange*) Given a group $G$ and a subgroup $S \le G$, denote with $[G:S]$ the number of cosets of $S$ in $G$. Then $\left\lvert G \right\rvert = [G:S] \cdot \left\lvert S \right\rvert$.

In order to prove this theorem, we need to do a bit of preparation.

## The number of left and right cosets is the same

It is best to start by showing that the theorem is valid. If the number of right and left cosets is equal, then it is fine to emit the factor position.

In order to do so, we can construct a bijection between the set of left and right cosets. Let $f$ be a function from the right cosets to the left cosets, and define it as $f(Sa) = a^{-1} S$.

It is simple prove that $f$ is a bijection. Consider the function $g$ from the left to right cosets, defined as $g(aS) = S a^{-1}$, and notice that $g$ is the inverse of $f$:
$$
(g \circ f) (Sa) = g(f(Sa)) = g(a^{-1} S) = S \left(a^{-1}\right)^{-1} = Sa
$$

therefore $f$ is a bijection, hence the number of left and right cosets is equal.

Notice also that this proof works for infinite sets of cosets (simply, replace any mention of "number" by "cardinality").

---

From this point, when referring to cosets we mean right cosets, although the statements are also true for left cosets.

## Cosets are either disjoint or equal

Consider two cosets $Sa, Sb$. We need to show that if $Sa \cap Sb \ne \varnothing$, then $Sa = Sb$. Let $x \in Sa \cap Sb$.

Because $x \in Sa$, then $x = s_1 a$ for some $s_1 \in S$. Similarly, because $x \in Sb$ then $x = s_2 b$ for some $s_2 \in S$. Comparing both expressions yields:

$$
\begin{aligned}
    s_1 a &= s_2 b \\
    s_1   &= s_2 b a^{-1} \\
    {s_2}^{-1} s_1 &= b a^{-1}
\end{aligned}
$$

Since $S$ is a group, and $s_1, s_2 \in S$, so is ${s_2}^{-1} s_1 \in S$. Therefore, $c = b a^{-1} \in S$. In order to show equality, we show double inclusion.

Let $y = sa \in Sa$. Notice that $s c^{-1} b = s a b^{-1} b = sa = y$. However, this also shows that $y \in Sb$, because $y = (sc^{-1}) b$. Similarly, for $y = sb \in Sb$, we have $y = sca$, and therefore $y \in Sa$. This shows that if $Sa \cap Sb \ne \varnothing$, then $Sa = Sb$. In other words, cosets are either disjoint or equal.

## Cosets partition $G$

Notice that for any $x \in G$, $x$ appears in exactly one coset. Simply, this is the coset $Sx$. Because cosets are either disjoint or equal, this means that there is no coset $Sx'$ for which $x \in Sx'$ *and* $Sx' \ne Sx$.

Because each element in $G$ appears in exactly one coset, then the set of all cosets is a partition of $G$.

## Cosets have equal size

Consider some coset $Sa$, and the original subgroup $S$. The function mapping from $S$ to $Sa$ defined by $f(s) = sa$ is a bijection. To show this, define the function $g$ mapping from $Sa$ to $S$ which is defined as $g(sa) = s$. It is easy to see that $g$ is the inverse of $f$:

$$
(g\circ f)(s) = g(sa) = s
$$
so $f$ is a bijection. This shows that $\left\lvert Sa \right\rvert = \left\lvert S \right\rvert$.

In particular, this is true for all cosets, and thus all cosets have the same size, of $\left\lvert S \right \rvert$.

## The Proof

Finally, we are ready to prove Lagrange's theorem.

Given $S \le G$, denote with $k = [G:S]$ the number of cosets (this number is also referred as the *index of $S$*). Because the cosets of $S$ partition $G$, then $G = S{a_1} \cup S{a_2} \cup \ldots \cup S{a_k}$ for some elements $a_1, \ldots, a_k$. Moreover, because cosets are disjoint, then this is actually a disjoint union. Therefore, $\left\lvert G \right \rvert = \sum_{1 \le i \le k} \left\lvert S {a_i} \right \rvert$. Finally, because $\left\lvert S {a_i} \right\rvert = \left\lvert S \right\rvert$, we have:

$$
 \begin{aligned}
    \left\lvert G \right\rvert &= \sum_{1 \le i \le k} \left\lvert S {a_i}  \right\rvert \\
                               &= \sum_{1 \le i \le k} \left\lvert S \right\rvert \\
                               &= k \cdot \left\lvert S \right\rvert = [G:S] \cdot \left\lvert S \right\rvert
 \end{aligned}
$$

$\blacksquare$
