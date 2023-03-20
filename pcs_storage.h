#include<gmp.h>
#include <inttypes.h>
#include "pcs_struct_hash.h"

Table_t *structInit(uint8_t memory, uint8_t is_dynamic);
uint8_t structAdd(Table_t *table, Tuple_t *tuple1, Tuple_t *tuple2);
void structFree(Table_t *table);