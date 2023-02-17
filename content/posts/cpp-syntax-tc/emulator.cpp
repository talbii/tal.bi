#include <fmt/core.h>
//#include <iostream>

/*
 * FRACTRAN emulation using C++ templates (because they are Turing complete)
 * */

// we cannot directly access
// fraction_type:value_struct<0>::value.
// With VS, we can indirect the access
// and thus access it. This is also true later.
template<typename fraction_type, int x>
using VS = typename fraction_type::template value_struct<x>;

struct end_list {
    template<int>
    struct value_struct { enum { value = -1 }; };
    enum { value = value_struct<0>::value };

    // -1 is a placeholder value.
    // FRACTRAN does not deal with
    // negative integers, so this is OK.

};

template<int x, int A, int B, typename next_fraction>
struct fraction {
    template<int x_>
    struct value_struct {
        enum {
            value = ( (x_ * A) % B == 0 ) ? // check if fraction * x_ is an integer
                (x_ * A) / B :      // if so, this is our match
                VS<next_fraction, x_>::value // otherwise, continue iterating the list
        };
    };

    enum { value = value_struct<x>::value };
};


template<int input, template<int> typename fraction_list>
struct program {
    enum { 
        output = (fraction_list<input>::value == -1) ? // another iteration returns -1?
               input : // if so, `input` holds our result
               program< fraction_list<input>::value, fraction_list >::output // otherwise, run another iteration
    };
};

// avoids infinite template-recursion:
template<template<int> typename T>
struct program<-1, T> { enum { output = -1 }; };

// define the program
template<int input>
using adder_program = fraction<input, 2, 3, end_list>;
// (note that template declarations 
//  are forbidden inside functions)

constexpr auto input = 2 * 2 * 2  *  3 * 3;

template<int p>
int get_register(int x) {
    int count = 0;
    while ( x % p == 0) x /= p, ++count;
    return count;
}

int main(void) {
    auto output = program<input, adder_program>::output;
    
    // expected output: 5
    
    fmt::print("3 + 2 is: {}\n", get_register<2>(output));
    // std::cout << "3 + 2 is: " << get_register<2>(output) << '\n';
}

