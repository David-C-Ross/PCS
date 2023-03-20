#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "pcs.h"
#include "pcs_storage.h"
#include "needle_pcs.h"
#include "needle_rho.h"
#include "random_functions.h"

static uint32_t search_space;
static uint32_t prob;

static uint32_t inner_flavor;
static uint32_t outer_flavor;

static uint8_t trailing_bits;

/** Determines whether a point is our needle.
 *
 *  @param[in]	collisions	    An array of possible needles.
 *  @param[in]	inverse_prob	1 / probability of needle.
 *  @param[in]	nb_collisions	number of possible needles to check.
 *  @return 	1 if one of the points is the needle, 0 otherwise.
 **/
int isNeedleMem(uint32_t *collisions, uint32_t inverse_prob, uint32_t nb_collisions) {

    uint8_t retval = 0;
    uint8_t *counters = calloc(nb_collisions, sizeof(uint8_t));

    uint32_t x;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, arc4random(), arc4random());

    inner_flavor = pcg32_boundedrand_r(&rng, search_space);

    for (uint32_t i = 0; i < 6 * inverse_prob; ++i) {
        x = pcg32_boundedrand_r(&rng, search_space);
        x = f(x, inner_flavor);
        for (int j = 0; j < nb_collisions; ++j) {
            if (collisions[j] == x) {
                counters[j]++;
                if (counters[j] >= 5) {
                    if (x == 1) {
                        retval = 1;
                    }
                    else {
                        retval = -1;
                    }
                    printf("%u is the needle!", collisions[j]);
                    goto end;
                }
            }
        }
    }
    end:
    free(counters);

    return retval;
}

uint32_t collisionPcs(Tuple_t *tuple1, Tuple_t *tuple2, uint8_t memory) {

    uint8_t flag;

    uint32_t x1, x2;
    uint32_t len1, len2;

    x1 = tuple1->start;
    x2 = tuple2->start;

    len1 = tuple1->length;
    len2 = tuple2->length;

    Table_t *inner_table1 = structInit(memory, 0);
    Table_t *inner_table2 = structInit(memory, 0);

    pcg32_random_t int_rng;
    pcg32_srandom_r(&int_rng, arc4random(), arc4random());

    while (len1 > len2) {
        pcsRun(inner_table1, x1, 1, &int_rng, &x1);
        x1 = hashInt(x1) ^ (search_space - 1);
        len1--;
    }
    while (len2 > len1) {
        pcsRun(inner_table2, x2, 1, &int_rng, &x2);
        x2 = hashInt(x2) ^ (search_space - 1);
        len2--;
    }
    if (x1 == x2) {
        flag = 0;
    } else {
        while (x1 != x2) {
            pcsRun(inner_table1, x1, 1, &int_rng, &x1);
            x1 = hashInt(x1) ^ (search_space - 1);

            pcsRun(inner_table2, x2, 1, &int_rng, &x2);
            x2 = hashInt(x2) ^ (search_space - 1);
        }
        flag = 1;
    }
    structFree(inner_table1);
    structFree(inner_table2);

    if (flag) {
        return x1;
    } else {
        return 0;
    }
}

uint32_t collisionRho(Tuple_t *tuple1, Tuple_t *tuple2) {

    uint8_t flag;

    uint32_t x1, x2;
    uint32_t len1, len2;

    x1 = tuple1->start;
    x2 = tuple2->start;

    len1 = tuple1->length;
    len2 = tuple2->length;

    while (len1 > len2) {
        x1 = x1 ^ outer_flavor;
        inner_flavor = x1;
        x1 = rho(x1, inner_flavor);
        len1--;
    }
    while (len2 > len1) {
        x2 = x2 ^ outer_flavor;
        inner_flavor = x2;
        x2 = rho(x2, inner_flavor);
        len2--;
    }
    // checks for robin hood
    if (x1 == x2) {
        flag = 0;
    } else {
        while (x1 != x2) {
            x1 = x1 ^ outer_flavor;
            inner_flavor = x1;
            x1 = rho(x1, inner_flavor);

            x2 = x2 ^ outer_flavor;
            inner_flavor = x2;
            x2 = rho(x2, inner_flavor);
        }
        flag = 1;
    }
    if (flag) {
        return x1;
    } else {
        return 0;
    }
}

uint8_t pcsPcsModeDetection(uint8_t nb_bits, uint8_t memory, uint8_t prob_init) {

    search_space = 1 << nb_bits;
    prob = 1 << prob_init;

    trailing_bits = trailingBitsInit(nb_bits, memory);

    uint32_t start, temp, collision, nb_collisions = 1 << memory;
    uint32_t *collisions = malloc(sizeof(uint32_t) * nb_collisions);

    uint32_t trail_length;
    uint32_t trail_length_max = (1 << trailing_bits) * 20;

    uint8_t counter, flag;

    Table_t *inner_table;
    Table_t *outer_table;

    Tuple_t *tuple1 = malloc(sizeof(Tuple_t));
    Tuple_t *tuple2 = malloc(sizeof(Tuple_t));

    pcg32_random_t ext_rng;
    pcg32_srandom_r(&ext_rng, arc4random(), arc4random());

    pcg32_random_t int_rng;

    initF(nb_bits, prob_init);

    do {
        counter = 0;

        outer_table = structInit(memory, 0);
        // create random outer_flavor
        outer_flavor = pcg32_boundedrand_r(&ext_rng, search_space);

        pcg32_srandom_r(&int_rng, arc4random(), arc4random());

        pcsInit(nb_bits, trailing_bits, outer_flavor);
        // find O(M) collisions which are also distinguished points
        for (int i = 0; i < nb_collisions; ++i) {

            start = pcg32_boundedrand_r(&ext_rng, search_space);
            temp = start;

            trail_length = 0;
            inner_table = structInit(memory, 0);

            do {
                pcsRun(inner_table, temp, 1, &int_rng, &collision);
                temp = hashInt(collision) ^ (search_space - 1);

                //printf("collision found!, %u \n", collision);
                trail_length++;

                if (trail_length > trail_length_max) {
                    start = pcg32_boundedrand_r(&ext_rng, search_space);
                    temp = start;

                    trail_length = 0;
                    structFree(inner_table);
                    inner_table = structInit(memory, 0);
                }
            } while (!isDistinguished(collision));

            tuple1->key = collision;
            tuple1->start = start;
            tuple1->length = trail_length;
            //printf("distinguished collision found!, %u \n", collision);
            structFree(inner_table);

            if (structAdd(outer_table, tuple1, tuple2)) {
                collision = collisionPcs(tuple1, tuple2, memory);
                if (collision) {
                    //printf("repeated collision!, %u \n", collision);
                    collisions[counter] = collision;
                    counter++;
                }
            }
        }
        structFree(outer_table);
        flag = isNeedleMem(collisions, prob, counter);
        if (flag) goto found_needle;

        //printf("________________________________ \n");
    } while (1);

    found_needle:

    free(collisions);
    free(tuple1);
    free(tuple2);

    return flag;
}

uint8_t pcsRhoModeDetection(uint8_t nb_bits, uint8_t memory, uint8_t prob_init) {

    search_space = 1 << nb_bits;
    prob = 1 << prob_init;

    trailing_bits = trailingBitsInit(nb_bits, memory);

    uint32_t start, collision, nb_collisions = 1 << memory;
    uint32_t *collisions = malloc(sizeof(uint32_t) * nb_collisions);

    uint32_t trail_length;
    uint32_t trail_length_max = (1 << trailing_bits) * 20;

    uint8_t counter, flag;

    Table_t *outer_table;

    Tuple_t *tuple1 = malloc(sizeof(Tuple_t));
    Tuple_t *tuple2 = malloc(sizeof(Tuple_t));

    pcg32_random_t ext_rng;
    pcg32_srandom_r(&ext_rng, arc4random(), arc4random());

    initF(nb_bits, prob_init);
    pcsInit(nb_bits, trailing_bits, 0);

    do {
        counter = 0;

        outer_table = structInit(memory, 0);
        // create random outer_flavor
        outer_flavor = pcg32_boundedrand_r(&ext_rng, search_space);
        // find O(M) collisions which are also distinguished points
        for (int i = 0; i < nb_collisions; ++i) {

            start = pcg32_boundedrand_r(&ext_rng, search_space);
            collision = start;
            trail_length = 0;

            do {
                collision =  collision ^ outer_flavor;
                inner_flavor = collision;
                collision = rho(collision, inner_flavor);
                trail_length++;

                //printf("collision found!, %u \n", collision);

                if (trail_length > trail_length_max) {
                    start = pcg32_boundedrand_r(&ext_rng, search_space);
                    collision = start;
                    trail_length = 0;
                }

            } while (!isDistinguished(collision));

            tuple1->key = collision;
            tuple1->start = start;
            tuple1->length = trail_length;
            //printf("distinguished collision found!, %u \n", collision);

            if (structAdd(outer_table, tuple1, tuple2)) {
                collision = collisionRho(tuple1, tuple2);
                if (collision) {
                    //printf("repeated collision!, %u \n", collision);
                    collisions[counter] = collision;
                    counter++;
                }
            }
        }
        structFree(outer_table);
        // flag = isNeedleMem(collisions, prob, counter);
        // if (flag) goto found_needle;
        for (int i = 0; i < counter; ++i) {
            if (collisions[i] == 1) {
                printf("%u is the needle!", collisions[i]);
                goto found_needle;
            }
        }

        //printf("________________________________ \n");
    } while (1);

    found_needle:

    free(collisions);
    free(tuple1);
    free(tuple2);

    return flag;
}

uint8_t pcsModeDetection(uint8_t nb_bits, uint8_t memory, uint8_t prob_init) {

    search_space = 1 << nb_bits;
    prob = 1 << prob_init;

    trailing_bits = trailingBitsInit(nb_bits, memory);

    uint32_t start, nb_collisions = 1 << memory;
    uint32_t *collisions = malloc(sizeof(uint32_t) * nb_collisions);

    uint8_t flag;

    pcg32_random_t rng;

    Table_t *outer_table;

    initF(nb_bits, prob_init);

    do {
        outer_table = structInit(memory, 1);

        pcg32_srandom_r(&rng, arc4random(), arc4random());

        start = pcg32_boundedrand_r(&rng, search_space);
        outer_flavor = pcg32_boundedrand_r(&rng, search_space);

        pcsInit(nb_bits, trailing_bits, outer_flavor);
        pcsRun(outer_table, start, nb_collisions, &rng, collisions);

        structFree(outer_table);

        flag = isNeedleMem(collisions, prob, nb_collisions);
        if (flag) goto found_needle;
    } while (1);

    found_needle:

    free(collisions);
    return flag;
}

