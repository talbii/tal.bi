#include <math.h>
#include <stdio.h>
#include <time.h>

#define MAX_POWER 40
#define fptr(NAME, RETURN_TYPE, ...) RETURN_TYPE (*NAME)(__VA_ARGS__)
#define TEST_FUNC(func) do {                                            \
        puts(##func);                                                   \
        for(unsigned int i = 2; i < MAX_POWER; i++) log_time(i, &func)  \
    while(0)                                                        

typedef unsigned long long ull;

static struct {
    double phi;
    ull fibonacci_numbers[MAX_POWER];
    size_t current_index; /*Pointer to the last non-zero cell*/
} program = {
    .fibonacci_numbers = {1, 1},
    .current_index = 1,
};

/*the three candidates*/
double regular_power(unsigned int);
double smart_power(unsigned int);
double lemma_power(unsigned int);
unsigned int get_fib(unsigned int);

void log_time(unsigned int i, fptr(f, double, unsigned int));

int main(void) {
    program.phi = (1 + sqrt(5))/2.0;


    TEST_FUNC(regular_power);
    TEST_FUNC(smart_power);
    TEST_FUNC(lemma_power);
}

double regular_power(unsigned int exponent) {
    if(exponent == 1) return program.phi;
    return program.phi * regular_power(exponent - 1);
}

double smart_power(unsigned int exponent) {
    if(exponent == 1) return program.phi;
    double res = smart_power(exponent/2);
    if(exponent % 2) {
        return res*res*program.phi;
    }

    return res * res;
}

double lemma_power(unsigned int exponent) {
    return get_fib(exponent - 1) * program.phi + get_fib(exponent - 2);
}

ull get_fib(unsigned int i) {
    if(i <= program.current_index) return program.fibonacci_numbers[i];
    
    ull a = program.fibonacci_numbers[program.current_index - 1],
        b = program.fibonacci_numbers[program.current_index];
    
    while(program.current_index++ < i) {
        program.fibonacci_numbers[program.current_index] = a + b;
        a = b;
        b = program.fibonacci_numbers[program.current_index];
    }

    program.current_index--; /*by current_index definition, it points to the last non-zero cell, so we need to decrement it.*/

    return program.fibonacci_numbers[i];
}

static long get_nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

void log_time(unsigned int i, double (*f)(unsigned int)) {
    long begin = get_nanos();
    double v = f(i);
    long end = get_nanos();
    printf("%ld\n",
           end - begin);
}
