#include <stdio.h>
#include <stdlib.h>
#include "needle_rho.h"
#include "pcs.h"
#include "pcg_basic.h"
#include "pcs_storage.h"
#include "random_functions.h"
#include "collision_storage.h"

static uint32_t search_space;
static uint32_t prob;

static uint32_t inner_flavor;
static uint32_t outer_flavor;

static uint8_t n;
static uint8_t trailing_bits;

int isNeedle(uint32_t collision, uint32_t inverse_prob) {

    uint8_t retval = 0, counter = 0;

    uint32_t x;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, arc4random(), arc4random());

    inner_flavor = pcg32_boundedrand_r(&rng, search_space);

    for (int i = 0; i < 5 * inverse_prob; ++i) {
        x = pcg32_boundedrand_r(&rng, search_space);
        x = f(x, inner_flavor);
        if (collision == x) {
            counter++;
            if (counter >= 4) {
                if (x == 1) {
                    retval = 1;
                } else {
                    retval = -1;
                }
                printf("%u is the needle!", collision);
                goto end;
            }
        }
    }

    end:
    return retval;
}

/** Determines whether a point is our needle.
 *
 *  @param[in]	collision	A possible needle.
 *  @param[in]	inverse_prob		1 / probability of needle.
 *  @return 	1 if one of the points is the needle, 0 otherwise.
 **/
int isNeedleRho(uint32_t collision, uint32_t inverse_prob) {

    uint8_t retval = 0, counter = 0;

    uint32_t x;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, arc4random(), arc4random());

    inner_flavor = pcg32_boundedrand_r(&rng, search_space);

    for (int i = 0; i < 6 * inverse_prob; ++i) {
        x = pcg32_boundedrand_r(&rng, search_space);
        x = rho(x, inner_flavor);
        if (collision == x) {
            counter++;
            if (counter >= 4) {
                if (x == 1) {
                    retval = 1;
                } else {
                    retval = -1;
                }
                printf("%u is the needle!", collision);
                goto end;
            }
        }
    }

    end:
    return retval;
}

int isNeedlePcs(uint32_t collision, uint32_t inverse_prob, uint8_t memory) {

    uint8_t retval = 0, counter = 0;

    uint32_t x;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, arc4random(), arc4random());

    inner_flavor = pcg32_boundedrand_r(&rng, search_space);

    Table_t *table = structInit(memory, 1);
    pcsInit(n, trailing_bits, inner_flavor);

    for (int i = 0; i < 3 * inverse_prob; ++i) {
        x = pcg32_boundedrand_r(&rng, search_space);
        pcsRun(table, x, 1, &rng, &x);
        if (collision == x) {
            counter++;
            if (counter >= 1) {
                if (x == 1) {
                    retval = 1;
                } else {
                    retval = -1;
                }
                printf("%u is the needle!", collision);
                goto end;
            }
        }
    }

    end:
    return retval;
}

uint8_t rhoModeDetection(uint8_t nb_bits, uint8_t prob_init) {

    n = nb_bits;
    search_space = 1 << nb_bits;
    prob = 1 << prob_init;  //(2 * prob_init - nb_bits);

    uint32_t start, collision;

    uint8_t flag;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, arc4random(), arc4random());

    initF(nb_bits, prob_init);

    do {
        start = pcg32_boundedrand_r(&rng, search_space);
        outer_flavor = pcg32_boundedrand_r(&rng, search_space);

        collision = nestedRho(start);

        // printf("collision found!, %u \n", collision);
        // printf("________________________________ \n");

        // flag = isNeedle(collision, prob);
        // if (flag) goto found_needle;
        if (collision == 1) {
            printf("%u is the needle!", collision);
            goto found_needle;
        }

    } while (1);

    found_needle:
    return flag;
}


uint8_t rhoPcsModeDetection(uint8_t nb_bits, uint8_t memory, uint8_t
prob_init) {

    n = nb_bits;
    search_space = 1 << nb_bits;
    prob = 1 << prob_init; //(2 * prob_init - nb_bits);

    trailing_bits = trailingBitsInit(nb_bits, memory);

    uint32_t start, collision;

    uint8_t flag;

    Table_t *inner_table;

    Tuple_t *tuple1 = malloc(sizeof(Tuple_t));
    Tuple_t *tuple2 = malloc(sizeof(Tuple_t));

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, arc4random(), arc4random());

    initF(nb_bits, prob_init);

    do {
        start = pcg32_boundedrand_r(&rng, search_space);
        outer_flavor = pcg32_boundedrand_r(&rng, search_space);

        inner_table = structInit(memory, 0);
        pcsInit(nb_bits, trailing_bits, outer_flavor);

        while (inner_table->memory_alloc < (1 << memory)) {
            getDistinguished(start, &rng, tuple1);
            structAdd(inner_table, tuple1, tuple2);
            start = pcg32_boundedrand_r(&rng, search_space);
        }

        collision = nestedPcs(inner_table, start);
        // printf("collision found!, %u \n", collision);
        // printf("________________________________ \n");
        structFree(inner_table);

        // flag = isNeedle(collision, prob);
        // if (flag) goto found_needle;
        if (collision == 1) {
            printf("%u is the needle!", collision);
            goto found_needle;
        }

    } while (1);

    found_needle:

    free(tuple1);
    free(tuple2);
    return flag;
}

uint32_t nestedRho(uint32_t start) {

    uint32_t tortoise, hare;
    uint32_t trail_length = 0, trail_length_max = (1 << (n / 2)) * 10;

    tortoise = start ^ outer_flavor;
    inner_flavor = tortoise;
    tortoise = rho(tortoise, inner_flavor);

    hare = tortoise ^ outer_flavor;
    inner_flavor = hare;
    hare = rho(hare, inner_flavor);

    while (tortoise != hare) {
        tortoise = tortoise ^ outer_flavor;
        inner_flavor = tortoise;
        tortoise = rho(tortoise, inner_flavor);
        //printf("tortoise!, %u \n", tortoise);

        hare = hare ^ outer_flavor;
        inner_flavor = hare;
        hare = rho(hare, inner_flavor);
        //printf("hare!, %lu \n", hare);
        hare = hare ^ outer_flavor;
        inner_flavor = hare;
        hare = rho(hare, inner_flavor);
        //printf("hare!, %lu \n", hare);

        trail_length++;
        if (trail_length > trail_length_max) return 0;
    }

    tortoise = start;
    while (tortoise != hare) {
        tortoise = tortoise ^ outer_flavor;
        inner_flavor = tortoise;
        tortoise = rho(tortoise, inner_flavor);

        hare = hare ^ outer_flavor;
        inner_flavor = hare;
        hare = rho(hare, inner_flavor);
    }
    return tortoise;
}

uint32_t rho(uint32_t start, uint32_t flavor) {

    uint32_t t, h;

    t = start;
    t = f(t, flavor);

    h = start;
    h = f(h, flavor);
    h = f(h, flavor);

    while (t != h) {
        t = f(t, flavor);

        h = f(h, flavor);
        h = f(h, flavor);
    }

    t = start;
    while (t != h) {
        t = f(t, flavor);
        h = f(h, flavor);
    }
    return t;
}

uint32_t nestedPcs(Table_t *table, uint32_t start) {

    uint32_t tortoise, hare, offset;
    uint32_t trail_length = 1, trail_length_max = (1 << (n / 2)) * 10;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, arc4random(), arc4random());

    offset = pcg32_boundedrand_r(&rng, search_space);

    tortoise = start ^ offset;
    pcsRun(table, tortoise, 1, &rng, &tortoise);

    hare = tortoise ^ offset;
    pcsRun(table, hare, 1, &rng, &hare);


    while (tortoise != hare) {
        tortoise = tortoise ^ offset;
        pcsRun(table, tortoise, 1, &rng, &tortoise);

        hare = hare ^ offset;
        pcsRun(table, hare, 1, &rng, &hare);
        hare = hare ^ offset;
        pcsRun(table, hare, 1, &rng, &hare);

        trail_length++;
        if (trail_length > trail_length_max) {
            return 0;
        }
    }
    tortoise = start;
    while (tortoise != hare) {
        tortoise = tortoise ^ offset;
        pcsRun(table, tortoise, 1, &rng, &tortoise);

        hare = hare ^ offset;
        pcsRun(table, hare, 1, &rng, &hare);
    }
    return tortoise;

}