#ifndef LIST_H
#define LIST_H

#include "Errors.h"

struct List {
    int fre;
    int capacity;
    int size;
    int *data;
    int *next;
    int *prev;
};

Type_Error list_ctor  (List *list);
Type_Error list_dump  (List *list);
Type_Error list_verify(List *list);
Type_Error list_dtor  (List *list);
Type_Error list_insert(List *list, int index, int value, int *new_index);
Type_Error list_erase (List *list, int index, int *value);
Type_Error list_get_el(List *list, int index, int *value);

#endif // LIST_H