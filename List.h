#ifndef LIST_H
#define LIST_H

#include <stdio.h>

#include "Errors.h"
#include "Config.h"

struct List {
    int fre;
    int capacity;
    int size;
    type_el *data;
    int *next;
    int *prev;
};

TypeError list_ctor        (List *list);
TypeError list_cmd_dump    (List *list);
TypeError list_graph_dump  (List *list, FILE *dump_file);
TypeError list_verify      (List *list);
TypeError list_dtor        (List *list);
TypeError list_insert      (List *list, int index, type_el value, int *new_index);
TypeError list_erase       (List *list, int index, type_el *value);
TypeError list_get         (List *list, int index, type_el *value);
TypeError list_get_index   (List *list, type_el value, int *index);
TypeError list_push_back   (List *list, type_el value, int *new_index);
TypeError list_push_front  (List *list, type_el value, int *new_index);
TypeError list_erase_back  (List *list, type_el *value);
TypeError list_erase_front (List *list, type_el *value);

#endif // LIST_H