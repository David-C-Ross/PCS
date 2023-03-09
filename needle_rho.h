#ifndef PCS_NEEDLE_RHO_H
#define PCS_NEEDLE_RHO_H

#include <inttypes.h>

void rhoModeDetection(uint8_t nb_bits, uint8_t prob_init);
uint32_t nestedRho(uint32_t start);
uint32_t rho(uint32_t start, uint32_t flavor);

#endif //PCS_NEEDLE_RHO_H
