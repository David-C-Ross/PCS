#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pcs.h"
#include "pcs_storage.h"
#include "random_functions.h"

static uint32_t search_space;
static uint32_t trailing_space;
static uint32_t flavor;

/** Determines whether a point is a distinguished one.
 *
 *  @param[in]	point				A point.
 *  @param[in]	trailing_bits  Number of trailing zero bits.
 *  @return 	1 if the point is distinguished, 0 otherwise.
 */
uint8_t isDistinguished(uint32_t point) {

    if (point % trailing_space == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void getDistinguished(uint32_t start, pcg32_random_t* rngptr, Tuple_t *tuple) {

    uint32_t x = start;
    uint32_t trail_length = 0;
    uint32_t trail_length_max = trailing_space * 20;

    while (!isDistinguished(x)) {
        x = f(x, flavor);
        trail_length++;

        if (trail_length > trail_length_max) {
            start = pcg32_boundedrand_r(rngptr, search_space);
            // start = hashInt(start) % search_space;

            x = start;
            trail_length = 0;
        }
    }
    //printf("distinguished point %u \n", x);
    tuple->key = x, tuple->start = start, tuple->length = trail_length;
}

/** Checks if there is a collision and returns the image.
 *
 */
uint32_t findCollision(Tuple_t *tuple1, Tuple_t *tuple2) {

    uint32_t x1, x2;
    uint32_t len1, len2;
    uint32_t pre1, pre2;

    x1 = tuple1->start;
    x2 = tuple2->start;

    len1 = tuple1->length;
    len2 = tuple2->length;

    while (len1 > len2) {
        x1 = f(x1, flavor);
        len1--;
    }
    while (len2 > len1) {
        x2 = f(x2, flavor);
        len2--;
    }
    // checks for robin hood
    if (x1 == x2) {
        return 0;
    }
    else {
        while (x1 != x2) {
            pre1 = x1;
            pre2 = x2;

            x1 = f(x1, flavor);
            x2 = f(x2, flavor);

        }
        //printf("start1: %u, start2: %u | f(%u) = f(%u) = %u \n",
        //       tuple1->start, tuple2->start, pre1, pre2, x1);

        return x1;
    }
}

/** Initialize all variables needed to do a PCS algorithm.
 *
 */
void pcsInit(uint8_t nb_bits, uint8_t trailing_bits, uint32_t flavor_init) {

    search_space = 1 << nb_bits;
    trailing_space = 1 << trailing_bits;
    flavor = flavor_init;
}

/** Run the PCS algorithm.
 *
 */
void pcsRun(Table_t *table, uint32_t  start, uint32_t nb_collisions,
            pcg32_random_t* rngptr, uint32_t *collisions) {

    uint32_t collision, i = 0;

    pcg32_srandom_r(rngptr, start, flavor);

    Tuple_t *tuple1 = malloc(sizeof(Tuple_t));
    Tuple_t *tuple2 = malloc(sizeof(Tuple_t));

    while(i < nb_collisions) {

        getDistinguished(start, rngptr, tuple1);

        if (structAdd(table, tuple1, tuple2)) {
            collision = findCollision(tuple1, tuple2);
            if (collision) {
                collisions[i] = collision;
                i++;
                // printf("collision found!, %u \n", collision);
            }
        }
        // get next starting point
        start = pcg32_boundedrand_r(rngptr, search_space);
        // start = hashInt(start) % search_space;

    }
    free(tuple1);
    free(tuple2);
}