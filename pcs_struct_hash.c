#include <math.h>
#include <stdlib.h>
#include "pcs_struct_hash.h"
#include "random_functions.h"

/** Initialize the hash table and allocate memory.
 *
 */
Table_t *structInitHash(uint8_t memory) {

    uint32_t i;
    uint32_t table_size = 1 << memory;

    //printf("table_size: %u \n",table_size);
    Table_t *table = malloc(sizeof(Table_t));
    Tuple_t **array = malloc(sizeof(*array) * table_size); //i.e. sizeof
    // (Tuple *)

    for (i = 0; i < table_size; i++) {
        array[i] = NULL;
    }

    table->array = array;
    table->table_size = table_size;
    //Memory limiting feature is turned on
    table->memory_alloc = 0;

    return table;
}

uint8_t structFindHash(Table_t *table, uint32_t point, Tuple_t *tuple) {

    uint32_t table_size = table->table_size;
    uint32_t i = hashInt(point) % table_size;

    Tuple_t *next;
    next = table->array[i];

    while (next != NULL && next->key != point) {
        next = next->next;
    }

    if(next != NULL && next->key == point) {
        // found collision
        tuple->start = next->start;
        tuple->length = next->length;
        return 1;
    }
    else {
        return 0;
    }
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
uint8_t structAddHash(Table_t *table, Tuple_t *tuple1, Tuple_t *tuple2) {

    uint32_t table_size = table->table_size;
    uint32_t i = hashInt(tuple1->key) % table_size;

    Tuple_t *new, *next, *last;
    next = table->array[i];

    while (next != NULL && next->key != tuple1->key) {
        last = next;
        next = next->next;
    }

    if(next != NULL && next->key == tuple1->key) {
        // found collision
        tuple2->start = next->start;
        tuple2->length = next->length;

        return 1;
    }

    // add new element to table
    new = malloc(sizeof(Tuple_t));
    new->key = tuple1->key;
    new->start = tuple1->start;
    new->length = tuple1->length;
    new->next = NULL;

    if (table->memory_alloc < table_size) {
        if(next == table->array[i]) {
            //add at the beginning
            new->next = next;
            table->array[i] = new;
        }
        else
        {
            if(next != NULL)  {
                //add in the middle
                new->next = next;
            }
            last->next = new;
        }
        table->memory_alloc++;
    }
    return 0;
}

/** Free the distinguished-point-storing structure.
 *
 */
void structFreeHash(Table_t *table) {

    uint32_t table_size = table->table_size;
    Tuple_t  *next, *last;

    for(uint32_t  i = 0; i < table_size; i++) {

        next = table->array[i];
        while(next != NULL)
        {
            last = next;
            next = next->next;
            free(last);
        }
    }
    free(table->array);
    free(table);
}
