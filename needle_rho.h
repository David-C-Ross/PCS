#ifndef PCS_NEEDLE_RHO_H
#define PCS_NEEDLE_RHO_H

#include <inttypes.h>
#include "pcs_struct_hash.h"

void initTrailLength(uint8_t n);
uint32_t rho(uint32_t start, uint32_t flavor);
uint32_t nestedRho(uint32_t start);
uint8_t rhoModeDetection(uint8_t n);

#endif //PCS_NEEDLE_RHO_H
