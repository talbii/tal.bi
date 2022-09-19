---
title: "Comments on Python 3.11 Integer/String Conversion Limits"
date: 2022-09-18
author: talbi
math: true
tags: ["python-3-11", "python", "opinions"]
---

Also why specification and implementation should be separate

<!--more-->

Recently CPython (the "official" implementation of the Python language) has [merged](https://github.com/python/cpython/pull/96500) an update which adds a bound to integer-to-string conversion. This means that the code `str(int(1e4301))` raises a `ValueError` instead of simply converting the number to a string. The reasoning given behind this change is to prevent DoS (denial of service) attacks on servers printing user input. This has sparked massive debate, and many people, including myself, believe this is a ridiculous update.

## The Good: Reasoning for the change

Currently CPython implements `int.__str__` using an algorithm which requires $O(d^2)$ steps, where $d$ is the number of digits in the number being converted (i.e., $d = \log_{10} x$). This means that the following code is vulnerable to a DoS attack:

```python
x = get_user_input()
server_log(f"Got user input: {x}.")
return_to_user(process_input(x))
```

if `get_user_input()` retrieves data over the internet (e.g., an online form) then the user is able to suspend the server for any amount of time simply by passing a large enough number. For example, printing the number $10^{200 \, 000}$ takes over 1 second on my machine. 

By limiting the integer/string conversion to numbers with $10^{4300}$ digits, one can ensure that this attack is nullified

## The Bad: Why this change is controversial

Whilst this change does fix the potential DoS attack possible on servers using Python, it does not make much sense. If one uses user input without checking it before, then only bad things could happen. A very strong argument is that if you do not check (and santize) user input before using it, then DoS attacks are the least of your concerns. The DoS attack this update is trying to fix could be avoided by santizing user input.

On the other hand, for people who use Python for anything other then backend, e.g., mathematicians simply suffer from this update. For example, if one wrote the code `print(f"Result = {very_expensive_calculation(x)}")` only to come back a day later to find a `ValueError` then it is probable that some bad words would been thrown at the screen.

Really, there is almost no scenario where this update makes sense. The people this update is helping is those who should not write backend code entirely (or instead learn some basic input santizing).

## Further Comments

I believe this change is bad for one simple reason - consistency. If we take a look at a language which I believe is better designed - C - then this change would, instead of deciding on a constant and a function to change it, instead it would have been mentioned in C's specification that there should be some limit. An important keyword here being specification.

Unfortunately, it appears today that many languages are merging specification and implementation, which I believe is something bad. Essentially, it disables creating multiple implementations for something (e.g., the standard library), since no one specifies what should happen in some edge cases. In constrast, a simple language like C (which has far less features then Python) has a specification whose PDF is over 500 pages. Whilst in most day-to-day applications issues with specific behaviour are rare, when they do happen it is often a matter of guessing, or checking the source code (a very tedious task!) instead of just checking the specification.

Finally, it appears as the update is nothing more then a bandage, covering a deeper issue of poor developers in today's market.

