#include <stdio.h>
#include <stdlib.h>
#include "needle_rho.h"
#include "pcg_basic.h"
#include "random_functions.h"

static uint32_t search_space;
static uint32_t prob;

static uint32_t inner_flavor;
static uint32_t outer_flavor;

/** Determines whether a point is our needle.
 *
 *  @param[in]	collision	A possible needle.
 *  @param[in]	inverse_prob		1 / probability of needle.
 *  @return 	1 if one of the points is the needle, 0 otherwise.
 **/
int isNeedle(uint32_t collision, uint32_t inverse_prob) {

    uint8_t retval = 0, counter = 0;

    uint32_t x;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, arc4random(), arc4random());

    for (int i = 0; i < inverse_prob; ++i) {
        x = pcg32_boundedrand_r(&rng, search_space);
        x = f(x, 0);
        if (collision == x) {
            counter++;
        }
    }
    if (counter >= 2) {
        printf("%u is the needle!", collision);
        retval =  1;
    }
    return retval;
}

void rhoModeDetection(uint8_t nb_bits, uint8_t prob_init) {

    search_space = 1 << nb_bits;
    prob = 1 << prob_init;

    uint32_t start, collision;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, arc4random(), arc4random());

    initF(nb_bits, prob);

    do {
        start = pcg32_boundedrand_r(&rng, search_space);
        outer_flavor = pcg32_boundedrand_r(&rng, search_space);

        collision = nestedRho(start);

        printf("collision found!, %u \n", collision);
        printf("________________________________ \n");
    }
    while (!isNeedle(collision, prob));
}

uint32_t nestedRho(uint32_t start) {

    uint32_t tortoise, hare;

    tortoise = start ^ outer_flavor;
    inner_flavor =  tortoise;
    tortoise = rho(tortoise, inner_flavor);

    hare = tortoise ^ outer_flavor;
    inner_flavor = hare;
    hare = rho(hare, inner_flavor);

    while (tortoise != hare) {
        tortoise = tortoise ^ outer_flavor;
        inner_flavor =  tortoise;
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
    }

    tortoise = start;
    while (tortoise != hare) {
        tortoise = tortoise ^ outer_flavor;
        inner_flavor =  tortoise;
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
