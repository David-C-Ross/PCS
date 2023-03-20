#include <inttypes.h>

typedef struct hashUNIX
{
    uint32_t collision;
    struct hashUNIX *next;

}hashUNIX_t;

void hashInit();
uint8_t hashAdd(uint32_t collision);
void hashFree();
