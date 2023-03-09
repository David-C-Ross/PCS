#include <gmp.h>
#include <inttypes.h>
#include "pcs_struct_hash.h"
#include "pcg_basic.h"

uint8_t isDistinguished(uint32_t point);
void getDistinguished(uint32_t start, pcg32_random_t* rngptr, Tuple_t *tuple);
void pcsInit(uint8_t nb_bits, uint8_t trailing_bits);
void pcsRun(Table_t *table, uint32_t flavor_init, uint32_t nb_collisions,
            pcg32_random_t* rngptr, uint32_t *collisions);