#ifndef PCS_STRUCT_HASH_H
#define PCS_STRUCT_HASH_H

#include <gmp.h>
#include <inttypes.h>

typedef struct Tuple
{
    uint32_t key;
    uint32_t start;
    uint32_t length;

}Tuple_t;

typedef struct Table
{
    Tuple_t ** array;
    uint32_t memory_alloc;

}Table_t;

Table_t *structInitHash(uint8_t memory);
uint8_t structAddHash(Table_t *table, Tuple_t *tuple1, Tuple_t *tuple2);
void structFreeHash(Table_t *table);

#endif