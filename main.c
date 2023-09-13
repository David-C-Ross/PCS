#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "pcs.h"
#include "needle_pcs.h"
#include "needle_rho.h"
#include "random_functions.h"

uint32_t search_space, memory, prob;

int main() {
    // the needle will appear 2^5=32 times, hash has been checked to not contain any other needles of similar magnitude.
    uint8_t n = 31, m = 5;

    double p = 22.5;

    search_space = 1 << n;
    memory = 1 << m;
    prob = (uint32_t) pow(2, p);

    FILE *fptr1;

    fptr1 = fopen("benchmarks1.txt","a");

    if(fptr1 == NULL) {
        printf("Error!");
        exit(1);
    }

    uint8_t flag;

    uint32_t nb_tests = 150;

    for (uint32_t i = 0; i < nb_tests; ++i) {

        printf("Test : %u \n", i);

        flag = rhoModeDetection(n);

        fprintf(fptr1,"%u : %u \n", i, flag);
        fflush(fptr1);
    }

    fclose(fptr1);

    return 0;
}