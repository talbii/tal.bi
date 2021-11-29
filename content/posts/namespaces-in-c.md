---
title: "Namespaces in C"
date: 2021-11-29T15:44:34+02:00
author: talbi
tags: ["c", "programming", "bad-ideas"]
---

<!--more-->

After showing you [how to add exceptions](/posts/exceptions-in-c), I will now show you how to add namespaces to C <sup>*</sup>

</sup>*</sup> this is a bad idea.

The solution I will showcase involves a `struct` which holds function pointers to all the functions in the scope. Obviously, an immediate issue that arises is the fact that your "namespace" cannot hold types, only *actual* variables.

In summary, here is how to create your own namespace, in C, right now!:

1. Create a (nameless) `struct` and give a variable of the type the name of your namespace
2. Create the `fptr` macro (*optional*) and declare all the (pointer) functions in your namespace
3. Initialize the variable's pointers with the desired functions

*fin*.

Here's how that looks:

```c++
/*brilliant macro, highly recommended*/

#define fptr(RETURN_VALUE, ptr_name, ...) RETURN_VALUE (*ptr_name)(__VA_ARGS__)

#include <stdio.h>
#include <stdlib.h> /*we must include these before initializing our namespace*/

struct {
    fptr(FILE*, fopen, const char *restrict, const char *restrict);
    fptr(int, fclose, FILE*);
    fptr(int, fprintf, FILE *restrict, const char *restrict, ...);
    fptr(int, printf, const char *restrict, ...);
    fptr(int, puts, const char*);
    fptr(void*, malloc, size_t);
    fptr(void, free, void*);
    const int esuccess;
    const int efailure;
    /*etc.*/
} std = {
    /*C99 designated initializer*/
    .fopen    = &fopen,
    .fclose   = &fclose,
    .fprintf  = &fprintf,
    .printf   = &printf,
    .puts     = &puts,
    .malloc   = &malloc,
    .free     = &free,
    .esuccess = EXIT_SUCCESS,
    .efailure = EXIT_FAILURE,
    /*...*/
};
```

and, using it is as simple as it can be (which is quite surprising!):

```c++
int main(void) {
    int x = 3;

    std.printf("%d\n", x);
    FILE* f = std.fopen("my_file.txt", "r");
  /*^^^^ you can't have types inside structs*/
    std.fprintf(f, "x = %d\n", x);
    std.fclose(f);
}
```

## Conclusions

- This is a very dumb idea, please don't use it
- If you *really* want namespaces, use C++
- If you **must** use C, then I'd recommend just naming your functions accordingly, such as `mylib_<some_function>` (which is how you should already be naming your functions)
- This is actually better then expected, and actually seems quite nice... *don't use this though*
