#include "stdlib.h"
#include "stdio.h"
#include <inttypes.h>
#include <time.h>
#include "needle_pcs.h"
#include "needle_rho.h"
#include "pcs_storage.h"
#include "pcs.h"
#include "random_functions.h"

int main() {
    // the needle will appear 2^5=32 times, hash has been checked to not contain any other needles of similar magnitude.
    uint8_t n = 25, memory = 6, prob = 20;

    // pcsPcsModeDetection(n, memory, prob);
    // rhoModeDetection(n, memory);
    // rhoPcsModeDetection(n, memory, prob);

    /*
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, arc4random(), arc4random());

    uint32_t start, search_space = 1 << n;
    uint32_t coll1, coll2;

    start = pcg32_boundedrand_r(&rng, search_space);

    Table_t *table = structInit(memory, 0);

    Tuple_t *tuple1 = malloc(sizeof(Tuple_t));
    Tuple_t *tuple2 = malloc(sizeof(Tuple_t));

    pcsInit(n, 6, 0);
    initF(n, prob, 1);

    while (table->memory_alloc < (1 << memory)) {
        getDistinguished(start, &rng, tuple1);
        structAdd(table, tuple1, tuple2);
        start = pcg32_boundedrand_r(&rng, search_space);
    }

    pcsRun(table, start, 1, &rng, &coll1);
    pcsRun(table, start, 1, &rng, &coll2);

    printf("%u \n", coll1);
    printf("%u \n", coll2);

    free(tuple1);
    free(tuple2);

    structFree(table);
*/
    FILE *fptr1;
    FILE *fptr2;
    FILE *fptr3;

    fptr1 = fopen("benchmarks1.txt","a");
    fptr2 = fopen("benchmarks2.txt","a");
    fptr3 = fopen("benchmarks3.txt","a");

    if(fptr1 == NULL || fptr2 == NULL || fptr3 == NULL) {
        printf("Error!");
        exit(1);
    }

    clock_t start_time;
    double elapsed_time, average_1 = 0, average_2 = 0, average_3 = 0;
    uint8_t flag;
    uint32_t nb_tests = 50;

    for (uint32_t i = 0; i < nb_tests; ++i) {

        printf("Test : %u \n", i);

        start_time = clock();

        flag = pcsRhoModeDetection(n, 5, prob);

        elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
        average_1 += elapsed_time;
        printf("done in %f seconds\n", elapsed_time);
        fprintf(fptr1,"%f : %u \n",elapsed_time, flag);
        fflush(fptr1);

        start_time = clock();

        flag = pcsRhoModeDetection(n, 10, prob);

        elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
        average_2 += elapsed_time;
        printf("done in %f seconds\n", elapsed_time);
        fprintf(fptr2,"%f : %u \n",elapsed_time, flag);
        fflush(fptr2);

        start_time = clock();

        flag = rhoPcsModeDetection(n, 10, prob);

        elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
        average_3 += elapsed_time;
        printf("done in %f seconds\n", elapsed_time);
        fprintf(fptr3,"%f : %u \n",elapsed_time, flag);
        fflush(fptr3);

    }

    printf("average time: %f \n", average_1 / nb_tests);
    printf("average time: %f \n", average_2 / nb_tests);
    printf("average time: %f \n", average_3 / nb_tests);

    fclose(fptr1);
    fclose(fptr2);
    fclose(fptr3);

    return 0;
}