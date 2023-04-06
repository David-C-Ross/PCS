#ifndef PCS_NEEDLE_RHO_H
#define PCS_NEEDLE_RHO_H

#include <inttypes.h>
#include "pcs_struct_hash.h"

uint8_t rhoModeDetection(uint8_t nb_bits, uint8_t prob_init);
uint8_t rhoPcsModeDetection(uint8_t nb_bits, uint8_t memory, uint8_t prob_init);
uint32_t nestedPcs(Table_t *table, uint32_t start);
uint32_t nestedRho(uint32_t start);
uint32_t rho(uint32_t start, uint32_t flavor);
void initRho(uint8_t nb_bits, uint8_t prob_init);


#endif //PCS_NEEDLE_RHO_H
