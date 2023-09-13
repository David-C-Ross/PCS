#include <math.h>
#include <stdlib.h>
#include "collision_storage.h"
#include "random_functions.h"

static uint32_t table_size = 1000;

/** Initialize the hash table and allocate memory.
 *
 */
Hash_t *initHash()
{
    Hash_t *hash = malloc(sizeof(Hash_t));
    Pair_t **array = malloc(sizeof(*array) * table_size);
    for(uint32_t i = 0; i < table_size; i++)
    {
        array[i] = NULL;
    }
    hash->array = array;
    return hash;
}

uint8_t findHash(Hash_t *hash, uint32_t point) {

    Pair_t *next;

    uint32_t i = hashInt(point) % table_size;
    next = hash->array[i];

    while (next != NULL && next->value != point) {
        // found collision
        next = next->next;
    }

    if(next != NULL && next->value == point) {
        return 1;
    }
    else {
        return 0;
    }
}

uint8_t addHash(Hash_t *hash, uint32_t point)
{
    Pair_t *new, *next, *last;

    uint32_t i = hashInt(point) % table_size;
    next = hash->array[i];

    while (next != NULL && next->value != point) {
        // found collision
        last = next;
        next = next->next;
    }

    if(next != NULL && next->value == point) {
        return 1;
    }
    else {

        // add new element to table
        new = malloc(sizeof(Pair_t));
        new->value = point;
        new->next = NULL;

        if(next == hash->array[i]) {
            //add at the beginning
            new->next = next;
            hash->array[i] = new;
        }
        else
        {
            if(next != NULL)  {
                //add in the middle
                new->next = next;
            }
            last->next = new;
        }
        return 0;
    }
}
void freeHash(Hash_t *hash)
{
    uint32_t i;
    Pair_t  *next, *last;

    for(i = 0; i < table_size; i++) {

        next = hash->array[i];
        while(next != NULL)
        {
            last = next;
            next = next->next;
            free(last);
        }
    }
    free(hash->array);
    free(hash);
}
