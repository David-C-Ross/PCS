#include <stdlib.h>
#include "pcs.h"
#include "pcs_struct_hash.h"
#include "needle_pcs.h"
#include "needle_rho.h"
#include "random_functions.h"

extern uint32_t search_space;
extern uint32_t memory;
extern uint32_t prob;

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
uint32_t isNeedleMem(const uint32_t *collisions, uint32_t nb_collisions) {

    uint8_t *counters = calloc(nb_collisions, sizeof(uint8_t));

    uint32_t x;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, rand(), rand());

    inner_flavor = pcg32_boundedrand_r(&rng, search_space);

    for (uint32_t i = 0; i < 4 * prob; ++i) {
        x = pcg32_boundedrand_r(&rng, search_space);
        x = f(x, inner_flavor);
        for (uint32_t j = 0; j < nb_collisions; ++j) {
            if (collisions[j] == x) {
                counters[j]++;
                if (counters[j] >= 2) {
                    free(counters);
                    return collisions[j];
                }
            }
        }
    }
    free(counters);
    return 0;
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

void pcsRhoModeDetection(uint8_t n, uint8_t m) {

    trailing_bits = trailingBitsInit(n, m);

    uint32_t start, collision, counter;

    uint32_t *collisions = malloc(sizeof(uint32_t) * memory);

    uint32_t trail_length;
    uint32_t trail_length_max = (1 << trailing_bits) * 20;

    Table_t *outer_table;

    Tuple_t *tuple1 = malloc(sizeof(Tuple_t));
    Tuple_t *tuple2 = malloc(sizeof(Tuple_t));

    pcg32_random_t ext_rng;
    pcg32_srandom_r(&ext_rng, rand(), rand());

    initTrailLength(n);

    do {
        counter = 0;

        outer_table = structInitHash(m);
        // create random outer_flavor
        outer_flavor = pcg32_boundedrand_r(&ext_rng, search_space);
        // find O(M) collisions which are also distinguished points
        for (int i = 0; i < memory; ++i) {

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

            } while (!isDistinguished(collision, trailing_bits));

            tuple1->key = collision;
            tuple1->start = start;
            tuple1->length = trail_length;
            // printf("distinguished collision found!, %u \n", collision);

            if (structAddHash(outer_table, tuple1, tuple2)) {
                collision = collisionRho(tuple1, tuple2);
                if (collision) {
                    //printf("repeated collision!, %u \n", collision);
                    collisions[counter] = collision;
                    counter++;
                }
                if (collision == 1) {
                    structFreeHash(outer_table);
                    goto found_needle;
                }
            }
        }
        // structFreeHash(outer_table);
        // needle = isNeedleMem(collisions, counter);
        // if (needle == 1) goto found_needle;

        /*
        for (int i = 0; i < counter; ++i) {
            if (collisions[i] == 1) {
                printf("%u is the needle!", collisions[i]);
                goto found_needle;
            }
        }
         */

        //printf("________________________________ \n");
    } while (1);

    found_needle:

    free(collisions);
    free(tuple1);
    free(tuple2);
}

void pcsModeDetection(uint8_t n, uint8_t m) {

    trailing_bits = trailingBitsInit(n, m);

    uint32_t start, needle;

    uint32_t *collisions = malloc(sizeof(uint32_t) * memory);

    pcg32_random_t rng;

    Table_t *outer_table;

    do {
        outer_table = structInitHash(m);

        pcg32_srandom_r(&rng, rand(), rand());

        start = pcg32_boundedrand_r(&rng, search_space);
        outer_flavor = pcg32_boundedrand_r(&rng, search_space);

        pcsInitFlavor(outer_flavor);
        pcsRun(outer_table, start, memory, trailing_bits, &rng, collisions);

        structFreeHash(outer_table);

        needle = isNeedleMem(collisions, memory);
        if (needle == 1) goto found_needle;

        /*
        for (int i = 0; i < nb_collisions; ++i) {
            if (collisions[i] == 1) {
                printf("%u is the needle!", collisions[i]);
                goto found_needle;
            }
        }
         */
    } while (1);

    found_needle:

    free(collisions);
}

