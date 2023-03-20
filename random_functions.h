#include <inttypes.h>

uint32_t jenkins(uint32_t a);
uint32_t hashInt(uint32_t a);
uint32_t f(uint32_t point, uint32_t flavor);
void initF(uint8_t nb_bits, uint8_t prob_init);
uint8_t trailingBitsInit(uint8_t nb_bits, uint8_t memory);