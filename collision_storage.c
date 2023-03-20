#include <math.h>
#include <stdlib.h>
#include <printf.h>
#include "collision_storage.h"
#include "random_functions.h"

static uint32_t table_size;
static hashUNIX_t **table;

/** Initialize the hash table and allocate memory.
 *
 */
void hashInit() {

    table_size = 1 << 10;

    //printf("table_size: %u \n",table_size);
    table = malloc(sizeof(*table) * table_size); //i.e. sizeof
    // (hashUnix_t *)

    for (uint32_t i = 0; i < table_size; i++) {
        table[i] = NULL;
    }
}


uint8_t hashAdd(uint32_t collision) {

    uint32_t h;
    hashUNIX_t *new, *last, *next;

    h = jenkins(collision) % table_size;
    next = table[h];
    while (next != NULL && next->collision != collision) {
        last = next;
        next = next->next;
    }

    if (next != NULL && next->collision == collision) //collision
    {
        return 1;
    }
    else
    {
        /***Memory limiting feature is turned off
        if(memory_alloc < memory_limit)
        {
         ***/
        new = malloc(sizeof(hashUNIX_t));
        new->collision = collision;
        new->next = NULL;

        if(next == table[h]) //add at the beginning
        {
            new->next = next;
            table[h] = new;
        }
        else
        {
            if(next != NULL) //add in the middle
            {
                new->next = next;
            }
            last->next = new;
        }
        //}
    }
    return 0;
}
void hashFree() {

    hashUNIX_t *last;
    hashUNIX_t *next;
    for(uint32_t i = 0; i < table_size; i++)
    {
        next = table[i];
        while(next != NULL)
        {
            last = next;
            next = next->next;
            free(last);
        }
    }
    free(table);
}