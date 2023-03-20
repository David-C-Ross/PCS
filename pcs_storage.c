#include<gmp.h>
#include "pcs_storage.h"

/** Initialize the distinguished-point-storing structure.
 * 
 */
Table_t *structInit(uint8_t memory, uint8_t is_dynamic)
{
    return structInitHash(memory, is_dynamic);
}

/** Search and insert.
 *  
 *  @brief Look for a point in the structure. If the point is not found 
 *  it is added with the corresponding start point.
 *
 *  @param[in]	tuple1	The newly added tuple (distinguished point, start
 *  point, trail length).
 *  @param[out]	tuple2	The found tuple (if exists in memory) otherwise NULL.
 *  @return 1 if the tuple is found in memory, 0 otherwise.
 */
uint8_t structAdd(Table_t *table, Tuple_t *tuple1, Tuple_t *tuple2) {

    return structAddHash(table, tuple1, tuple2);
}

/** Free the distinguished-point-storing structure.
 * 
 */
void structFree(Table_t *table) {

    structFreeHash(table);
}
