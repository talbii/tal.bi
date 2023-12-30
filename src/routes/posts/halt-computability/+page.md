---
title: "Notes on the Halting Problem"
author: talbi
date: 2023-12-07T12:52:13+02:00
tags: ["math", "computability", "turing"]
math: true
---

<!--more-->

The computability of the $\mathrm{HALT}$ class is a simple problem to solve, which provides a very powerful result. Mainly, that we cannot prove that arbitrary Turing machines halt or not.

Except that this is not accurate. The undecidable nature of $\mathrm{HALT}$ merely says that no singular Turing machine (i.e., a single algorithm) can compute whether any Turing machine halts or not.

Proving this is a fun little exercise, and the proof is similar to proving Russel's paradox.

## Proving $\mathrm{HALT}$ is undecidable

We begin by assuming that there is some Turing machine, $H$, which can decide whether a given input $\langle M, x \rangle$ halts or not (running $M$ with input $x$). Then, construct a Turing machine $C$ as follows:

- Given input $x$:
  - If $\langle x, x \rangle$ halts, then loop forever.
  - Otherwise, halt.

Where checking if $\langle x, x \rangle$ halts is done using $H$. Now, consider what happens when we run $C$ with the input $C$ (i.e., feed $C$ to itself):

- If $\langle C, C\rangle$ halts, then by the construction of $C$, $C$ doesn't halt.
- If $\langle C, C\rangle$ doesn't halt, then again by the construction, $C$ should halt.

This results in a contradiction, with the contradiction being the existence of $H$ itself.

While this proof is simple and effective, it does not give much insight to this problem. In fact, here is an algorithm which supposedly contradicts this proof:

- Given input $\langle M, x \rangle$:
  - Enumerate on all proofs of $\langle M, x \rangle$, alternating between proving halting, and proving non-halting (infinite loop)
  - Because $M$ must either halt or loop forever, eventually some proof will be found. Via this proof, report back "YES"/"NO", depending on what proof we found.

Because proofs are always finite, this means that the algorithm above must stop sometime, and report a result. Yet, this contradicts what we've just proved: $\mathrm{HALT}$ is undecidable, but here is an algorithm which decides it.

## Gödel's Incompleteness Theorems

In 1931, Gödel published an article showcasing two incredible results in mathematical logic. Of those two, the first is relevant to us. The result, also known as *Gödel's first incompleteness theorem*, states:

> **Theorem.** (*Gödel's first incompleteness theorem*) Given any formal system $F$, there exist some statement $G$ (also known as Gödel's sentence) which cannot be proven or disproven under $F$.

The connection between Gödel's theorem and the halting problem is striking. Given any supposed algorithm $H$ which decides $\mathrm{HALT}$, we can find some Turing machine for which $H$ simply cannot decide whether it halts or not. However, this time no clever constructions are needed.

Because we assume $H$ to be correct (that is, answer correctly and not randomly), this means that $H$ must perform a sequence of logical conclusions, in order to arrive at the answer of whether a given machine halts or not. This is essentially the formal system, $F_H$, under which $H$ is working. From Gödel's first incompleteness theorem we can draw the conclusion that there is some statement $G_H$, which is unprovable under $F_H$. Finally, it is possible to turn $G_H$ into a Turing machine, by halting iff $G_H$ is true, and looping forever if it is false.

The algorithm above fails in deciding $\mathrm{HALT}$, because such proof might not exist, hence causing the algorithm to loop forever. For the sake of completeness: such algorithm cannot exist, because for any algorithm we can encode it as a proof under some formal system $F$, and for each such system there is an unprovable statement.

Gödel's incompleteness theorems were published before the consideration of the halting problems. At the time of publishing, this created a huge shock, as it implied that some statements simply cannot be proved (roughly speaking). For me, using these theorems in the context of computability is as shocking: it essentially implies that there exist Turing machine on which we cannot prove anything.

Of course, looking at this from a higher-level standpoint is not very impressive; Turing machines can be thought of as just proof enumerations, and in that regard indeed the two results are exactly the same.

## Computability vs Decidability

Finally, a word on search problems. A similar construction can build *uncomputable functions*. That is, functions $f : \mathbb{N} \to \mathbb{N}$ for which no Turing machine can compute $f(x)$, for sufficient $x$. To do this, we utilize unprovable statements, yet again. Let $f$ be some function, and $G$ some Gödel statement. We construct the uncomputable function $f_G : \mathbb{N} \to \mathbb{N} \cup \{ \perp \}$ by:

$$
f_G(x) = \begin{cases}
    f(x) && G \text{ is provable} \\
    \perp && \text{otherwise}
\end{cases}
$$

Thus, for example, being able to compute *any value* of $f_G$ would be equivalent to the Turing machine proving $G$. In other words, any output of an evaluation of $f_G$ at any input would require either proving or disproving $G$.

$f_G$ might perhaps be cheating, as the existence of $G$ is dependent on the machine itself, yet we require the machine in order to construct the function. Instead, let us construct a function in two parts. First, let $g_F : \mathbb{N} \times \mathsf{Stmt}(F) \to \mathbb{N} \cup \{ \perp \}$ be defined as:

$$
g_F(x, \varphi) = \begin{cases}
    x && \varphi \text{ is provable} \\
    \perp && \text{otherwise}
\end{cases}
$$

It is clear that $g_F$ is computable *for some inputs*, such as $x \in \mathbb{N}$ and $\varphi = \mathbf{true}$, yet not computable for all inputs. Indeed, for $\varphi = G$ the function $g^{\prime} (x) = g_F(x, G)$ is not computable by the same machine that might compute some of $g_F$. Instead, construct a function $h$ which is not computable at all, by:

$$
h(x) = \sum_{\substack{\varphi \in \mathsf{Stmt}(F) \\ \lvert \varphi \rvert \le x}} g_F(x, \varphi)
$$

Which by itself is partially computable, yet $h^{\prime} (x) = h(x + \lvert G \rvert)$ with $G$ being the shortest Gödel statement under $F$. $h^\prime$ is not computable under any input.
