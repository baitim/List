#ifndef LIST_H
#define LIST_H

#include "Errors.h"

const int MAX_SIZE_LIST = 10;
const int POISON_EL = -0xbe;

struct LIST {
    int capacity;
    int size;
    int *data;
    int *next;
    int *prev;
};

TypeError list_ctor(LIST *list);
TypeError list_dump(LIST *list);
TypeError list_verify(LIST *list);
TypeError list_dtor(LIST *list);
TypeError list_push(LIST *list, int index);
TypeError list_pop(LIST *list);

#endif // LIST_H