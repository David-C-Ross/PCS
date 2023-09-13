#include <inttypes.h>
#include "pcs_struct_hash.h"
#include "pcg_basic.h"

uint8_t isDistinguished(uint32_t point, uint8_t trailing_bits);
void getDistinguished(uint32_t start, uint8_t trailing_bits, pcg32_random_t* rngptr, Tuple_t *tuple);
uint32_t findCollision(Tuple_t *tuple1, Tuple_t *tuple2);
void pcsInitFlavor(uint32_t flavor_init);
void pcsRun(Table_t *table, uint32_t  start, uint32_t nb_collisions, uint8_t trailing_bits,
            pcg32_random_t* rngptr, uint32_t *collisions);