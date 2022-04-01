/*Showcase of the new Reference feature in C22*/
#include <stdio.h>
#include <stdlib.h>
#include "reference.h" /*C22 feature*/

void swap(int ref a, int ref b);

int main(void) {
    int a, b;
    int n;
    if((n = scanf("%d %d", get_ref(a), get_ref(b))) != 2) {
        fprintf(stderr, "Missing values; got %d values (expected: 2)\n", n);
        exit(EXIT_FAILURE);
    }

    printf("a = %d, b = %d\n", a, b);
    swap(get_ref(a), get_ref(b));
    printf("a = %d, b = %d\n", a, b);
}

void swap(int ref a, int ref b) {
    int t = ref a;
    ref a = ref b;
    ref b = t;
}
