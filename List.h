#ifndef LIST_H
#define LIST_H

#include <stdio.h>

#include "Errors.h"

struct List {
    int fre;
    int capacity;
    int size;
    int *data;
    int *next;
    int *prev;
};

Type_Error list_ctor        (List *list);
Type_Error list_cmd_dump    (List *list);
Type_Error list_graph_dump  (List *list, FILE *dump_file);
Type_Error list_verify      (List *list);
Type_Error list_dtor        (List *list);
Type_Error list_insert      (List *list, int index, int value, int *new_index);
Type_Error list_erase       (List *list, int index, int *value);
Type_Error list_get         (List *list, int index, int *value);
Type_Error push_back        (List *list, int value, int *new_index);
Type_Error push_front       (List *list, int value, int *new_index);
Type_Error erase_back       (List *list, int *value);
Type_Error erase_front      (List *list, int *value);

#endif // LIST_H