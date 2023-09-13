#include <stdio.h>
#include <stdlib.h>
#include "needle_rho.h"
#include "pcs.h"
#include "pcg_basic.h"
#include "pcs_struct_hash.h"
#include "random_functions.h"

extern uint32_t search_space;
extern uint32_t prob;

static uint32_t inner_flavor;
static uint32_t outer_flavor;

static uint32_t trail_length_max;

void initTrailLength(uint8_t n) {

    trail_length_max = (1 << (n / 2)) * 10;
}

uint32_t rho(uint32_t start, uint32_t flavor) {

    uint32_t t, h;
    uint32_t trail_length = 0;

    t = start;
    t = f(t, flavor);

    h = start;
    h = f(h, flavor);
    h = f(h, flavor);

    while (t != h) {
        t = f(t, flavor);

        h = f(h, flavor);
        h = f(h, flavor);

        trail_length++;
        if (trail_length > trail_length_max) return rho(start + 1, flavor);
    }

    t = start;
    while (t != h) {
        t = f(t, flavor);
        h = f(h, flavor);
    }
    return t;
}

uint32_t nestedRho(uint32_t start) {

    uint32_t tortoise, hare;
    uint32_t trail_length = 0;

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
    //printf("trail_length %u \n", trail_length);
    return tortoise;
}

uint8_t rhoModeDetection(uint8_t n) {

    uint32_t start, collision;

    pcg32_random_t rng;
    pcg32_srandom_r(&rng, rand(), rand());

    initTrailLength(n);

    do {
        start = pcg32_boundedrand_r(&rng, search_space);
        outer_flavor = pcg32_boundedrand_r(&rng, search_space);

        collision = nestedRho(start);

        // printf("collision found!, %u \n", collision);
        // printf("________________________________ \n");

        /*
        if (collision == 1) {
            printf("%u is the needle!", collision);
            return collision;
        }
         */

        return (collision == 1);

    } while (1);

}