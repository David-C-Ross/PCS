#ifndef PCS_NEEDLE_PCS_H
#define PCS_NEEDLE_PCS_H

#include <inttypes.h>

uint8_t pcsPcsModeDetection(uint8_t nb_bits, uint8_t memory, uint8_t prob_init);
uint8_t pcsRhoModeDetection(uint8_t nb_bits, uint8_t memory, uint8_t prob_init);
uint8_t pcsModeDetection(uint8_t nb_bits, uint8_t memory, uint8_t prob_init);

#endif