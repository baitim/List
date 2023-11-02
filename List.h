#ifndef LIST_H
#define LIST_H

#include "Errors.h"

struct LIST {
    int capacity;
    int size;
    int *data;
    int *next;
    int *prev;
};

TYPE_ERROR list_ctor  (LIST *list);
TYPE_ERROR list_dump  (LIST *list);
TYPE_ERROR list_verify(LIST *list);
TYPE_ERROR list_dtor  (LIST *list);
TYPE_ERROR list_push  (LIST *list, int index, int value);
TYPE_ERROR list_pop   (LIST *list, int index, int *value);

#endif // LIST_H