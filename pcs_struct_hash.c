#include <math.h>
#include <stdlib.h>
#include <printf.h>
#include "pcs_struct_hash.h"

static uint32_t table_size;

/** Initialize the hash table and allocate memory.
 *
 */
Table_t *structInitHash(uint8_t memory) {

    uint32_t i;
    table_size = 1 << memory;

    //printf("table_size: %u \n",table_size);
    Table_t *table = malloc(sizeof(Table_t));
    Tuple_t **array = malloc(sizeof(*array) * table_size); //i.e. sizeof
    // (Tuple *)

    for (i = 0; i < table_size; i++) {
        array[i] = NULL;
    }

    table->array = array;
    //Memory limiting feature is turned on
    table->memory_alloc = 0;

    return table;
}

uint8_t structAddHash(Table_t *table, Tuple_t *tuple1, Tuple_t *tuple2) {

    uint32_t i, found = 0;
    Tuple_t *new, *next;

    for (i = 0; i < table_size; ++i) {
        next = table->array[i];

        if (next != NULL && next->key == tuple1->key) {
            // found collision
            tuple2->start = next->start;
            tuple2->length = next->length;

            next->start = tuple1->start;
            next->length = tuple1->length;

            return 1;
        }
    }
    // add new element to table
    new = malloc(sizeof(Tuple_t));
    new->key = tuple1->key;
    new->start = tuple1->start;
    new->length = tuple1->length;

    if (table->memory_alloc < table_size) {
        table->array[table->memory_alloc] = new;
    } else {
        i = table->memory_alloc % table_size;
        free(table->array[i]);
        table->array[i] = new;
    }
    table->memory_alloc++;

    return 0;
}

void structFreeHash(Table_t *table) {

    for (uint32_t i = 0; i < table_size; i++) {
        if (table->array[i] != NULL) {
            free(table->array[i]);
        }
    }
    free(table->array);
    free(table);
}
