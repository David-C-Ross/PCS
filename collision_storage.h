#ifndef COLLISION_STORAGE_H
#define COLLISION_STORAGE_H

#include <inttypes.h>

typedef struct Pair
{
    uint32_t value;
    struct Pair *next;

}Pair_t;

typedef struct Hash
{
    Pair_t **array;
}Hash_t;

Hash_t *initHash();
uint8_t findHash(Hash_t *hash, uint32_t point);
uint8_t addHash(Hash_t *hash, uint32_t point);
void freeHash(Hash_t *hash);

#endif