#include <inttypes.h>

static uint32_t search_space;
static uint32_t prob;

uint32_t jenkins(uint32_t a)
{
    a = (a+0x7ed55d16) + (a<<12);
    a = (a^0xc761c23c) ^ (a>>19);
    a = (a+0x165667b1) + (a<<5);
    a = (a+0xd3a2646c) ^ (a<<9);
    a = (a+0xfd7046c5) + (a<<3);
    a = (a^0xb55a4f09) ^ (a>>16);
    return a;
}

uint32_t hashInt(uint32_t a)
{
    a += ~(a<<15);
    a ^=  (a>>10);
    a +=  (a<<3);
    a ^=  (a>>6);
    a += ~(a<<11);
    a ^=  (a>>16);
    return a;
}

/** Evaluate the function f at a given point.
 *
 * 	@param[in]	point	    Current point.
 */
uint32_t f(uint32_t point, uint32_t flavor) {

    // the flavor of f
    point = point ^ flavor;

    uint32_t i = hashInt(point);
    uint32_t j = jenkins(point);

    if (i % prob == 0) {
        return 1;
    } else {
        j = j & (search_space - 1);
        return j;
    }
}

void initF(uint8_t nb_bits, uint8_t prob_init) {
    search_space = 1 << nb_bits;
    prob = 1 << prob_init;
}

uint8_t trailingBitsInit(uint8_t nb_bits, uint8_t memory) {
    uint8_t bits = nb_bits - memory;
    return bits / 2;
}
