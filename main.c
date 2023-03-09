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
    uint8_t n = 23, memory = 6, prob = 18;

    //pcsPcsModeDetection(n, memory, prob);
    //pcsRhoModeDetection(n, memory, prob);
    //pcsModeDetection(n, memory, prob);
/*
    pcg32_random_t rng1;
    pcg32_srandom_r(&rng1, arc4random(), arc4random());

    Table_t *table;
    table = structInit(memory);

    uint32_t flavor_init = 0, nb_collisions = 10000;
    uint32_t *collisions = malloc(sizeof(uint32_t) * nb_collisions);

    pcsInit(n, 8);
    initF(n, prob);
    pcsRun(table, flavor_init, nb_collisions, &rng1, collisions);

    for (int i = 0; i < nb_collisions; ++i) {
        printf("%u \n", collisions[i]);
    }

    free(collisions);
    structFree(table);

 */
    FILE *fptr1;
    FILE *fptr2;
    FILE *fptr3;

    fptr1 = fopen("benchmarksPCS16.txt","a");
    fptr2 = fopen("benchmarksRho16.txt","a");
    fptr3 = fopen("benchmarks16.txt","a");

    if(fptr1 == NULL || fptr2 == NULL || fptr3 == NULL) {
        printf("Error!");
        exit(1);
    }

    clock_t start_time;
    double elapsed_time, average_pcs = 0, average_rho = 0, average = 0;
    uint8_t flag, nb_tests = 100;

    for (uint8_t i = 0; i < nb_tests; ++i) {

        printf("Test : %u \n", i);

        start_time = clock();

        flag = pcsPcsModeDetection(n, memory, prob);

        elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
        average_pcs += elapsed_time;
        printf("PCS (nested) done in %f seconds\n", elapsed_time);
        fprintf(fptr1,"%f : %u \n",elapsed_time, flag);
        fflush(fptr1);

        start_time = clock();

        flag = pcsRhoModeDetection(n, memory, prob);

        elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
        average_rho += elapsed_time;
        printf("Rho (nested) done in %f seconds\n", elapsed_time);
        fprintf(fptr2,"%f : %u \n",elapsed_time, flag);
        fflush(fptr2);

        start_time = clock();

        flag = pcsModeDetection(n, memory, prob);

        elapsed_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;
        average += elapsed_time;
        printf("PCS done in %f seconds\n", elapsed_time);
        fprintf(fptr3,"%f : %u \n",elapsed_time, flag);
        fflush(fptr3);
    }

    printf("average time PCS (nested): %f \n", average_pcs / nb_tests);
    printf("average time RHO (nested): %f \n", average_rho / nb_tests);
    printf("average time PCS : %f \n", average / nb_tests);

    fclose(fptr1);
    fclose(fptr2);
    fclose(fptr3);

    return 0;
}