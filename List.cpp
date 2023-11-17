#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "List.h"

#define MAX(a, b) ((a) > (b)) ? (a) : (b)

const int DEFAULT_SIZE = 0;
const int DEFAULT_CAPACITY = 1;
const double MULTIPLIER_CAPACITY = 2;
const int POISON_INT = 0xbe;
const int MAX_SIZE_NAME_DUMP = 50;
const int MAX_SIZE_COMMAND = 100;

static TypeError list_increase(List *list);
static TypeError list_reduce(List *list, List *new_list, int *index);
static void copy_valid(List *list, List *new_list, int *index);
static void set_poison(type_el *a, int size);
static void set_index_next(int *a, int st, int size);
static void set_index_prev(int *a, int size);

TypeError list_ctor(List *list)
{
    if (!list)
        return ERROR_INVALID_LIST;

    list->free = 1;
    list->capacity = DEFAULT_CAPACITY;
    list->size = DEFAULT_SIZE;

    list->data = (type_el *)calloc(list->capacity, sizeof(type_el));
    if (!list->data)
        return ERROR_ALLOC_FAIL;
    set_poison(list->data, list->capacity);

    list->next = (int *)calloc(list->capacity, sizeof(int));
    if (!list->next)
        return ERROR_ALLOC_FAIL;
    set_index_next(list->next, 0, list->capacity);
    list->next[0] = 0;

    list->prev = (int *)calloc(list->capacity, sizeof(int));
    if (!list->prev)
        return ERROR_ALLOC_FAIL;
    set_index_prev(list->prev, list->capacity);
    list->prev[0] = 0;

    return list_verify(list);
}

TypeError list_cmd_dump(List *list)
{
    if (!list)
        return ERROR_INVALID_LIST;

    TypeError err = list_verify(list);
    if (err)
        return err;

    for (int i = 0; i < list->capacity * (int)sizeof(double); i++)
        printf("v");
    printf("\nLIST:\n\n");

    printf("free = %d\n", list->free);
    printf("SIZ = %d\n", list->size);
    printf("CAP = %d\n", list->capacity);

    for (int i = 0; i < list->capacity; i++) {
        printf("%d\t", i);
    }
    printf("\n");
    for (int i = 0; i < list->capacity; i++) {
        print_el(&list->data[i]);
    }
    printf("\n");
    for (int i = 0; i < list->capacity; i++) {
        printf("%d\t", list->next[i]);
    }
    printf("\n");
    for (int i = 0; i < list->capacity; i++) {
        printf("%d\t", list->prev[i]);
    }
    printf("\n");
    for (int i = 0; i < list->capacity * (int)sizeof(double); i++)
        printf("^");
    printf("\n");

    return list_verify(list);
}

// enum GraphvizNodeShape
// {
// };
// struct GraphvizNode
// {
//     const char* label;
//     GraphvizNodeShape shape;
// }

TypeError list_graph_dump(List *list, int *number_grpah_dump)
{
    char buffer[MAX_SIZE_NAME_DUMP] = "";
    snprintf(buffer, MAX_SIZE_NAME_DUMP, "dumps/dump%d", *number_grpah_dump);

    FILE *dump_file = fopen(buffer, "w");
    if (!dump_file) {
        printf("Error open file to dump\n");
        return ERROR_OPEN_FILE;
    }
    
    if (!list) return ERROR_INVALID_LIST;

    TypeError err = list_verify(list);
    if (err)
        return err;

    fprintf(dump_file, "digraph {\n"
                       "\tgraph[label = \"List\", labelloc = top, "
                       "labeljust = center, fontsize = 70, fontcolor = \"#e33e19\"];\n"
                       "\tgraph[dpi = 140];\n"
	                   "\tsplines = ortho;\n"
                       "\tbgcolor = \"#2F353B\"\n"
                       "\tedge[minlen = 3, penwidth = 3];\n");

    fprintf(dump_file, "\tnode[shape = \"rectangle\", style = \"rounded, filled\", height = 3, width = 2, ");
	fprintf(dump_file, "fillcolor = \"#ab5b0f\", fontsize = 30, penwidth = 3.5, color = \"#941b1b\"]\n");

    fprintf(dump_file, "\t{ rank = same;\n");
    fprintf(dump_file, "\t\tnode[shape = \"Mrecord\"];\n");
    for (int i = 1; i < list->capacity; i++) {
        fprintf(dump_file, "\t\tnode%d[label = \"{ id %d | ", i, i);
        print_dump_el(&list->data[i], dump_file);
        fprintf(dump_file, " | next %d | prev %d }\"", list->next[i], list->prev[i]);
        if (list->prev[i] == -1)
            fprintf(dump_file, ", fillcolor = \"#e3964d\"");
        fprintf(dump_file, "];\n");
    }
    fprintf(dump_file, "\t}\n");

    fprintf(dump_file, "\t{ rank = min;\n");
    fprintf(dump_file, "\t\tnode[shape = \"Mrecord\"];\n");
    fprintf(dump_file, "\t\tnode0[label = \"{ id 0 | ");
    print_dump_el(&list->data[0], dump_file);
    fprintf(dump_file, " | next %d | prev %d }\", fillcolor = \"#bf9732\"];\n", list->next[0], list->prev[0]);
    fprintf(dump_file, "\t}\n");

    fprintf(dump_file, "\t{ rank = max;\n");
    fprintf(dump_file, "\t\tnode_free[label = \"free\", fillcolor = \"#e3964d\"];\n");
    fprintf(dump_file, "\t}\n");

    fprintf(dump_file, "\tnode_free->node%d [color = \"#cf3000\"];\n", list->free);

    for (int i = 0; i < list->capacity - 1; i++) {
        fprintf(dump_file, "\tnode%d->node%d[weight = 100, style = invis];\n", i, i + 1);
    }

    for (int i = 0; i < list->capacity; i++) {
        if (list->prev[i] == -1) {
            fprintf(dump_file, "\tnode%d->node%d[color = \"#cf3000\"];\n", i, list->next[i]);
        } else {
            fprintf(dump_file, "\tnode%d->node%d[color = yellow];\n", i, list->next[i]);
            fprintf(dump_file, "\tnode%d->node%d[color = \"#fada69\"];\n", list->next[i], i);
        }
    }

    fprintf(dump_file, "}\n");

    (*number_grpah_dump)++;
    fclose(dump_file);

    char command[MAX_SIZE_COMMAND] = "";
    snprintf(command, MAX_SIZE_COMMAND, "gvpack -u %s | dot -Tpng -o %s.png", buffer, buffer);
    int sys = system(command);
    if (sys) 
        return ERROR_SYSTEM_COMMAND;

    return list_verify(list);
}

TypeError list_html_dump(int number_grpah_dump)
{
    char buffer[MAX_SIZE_NAME_DUMP] = "";
    snprintf(buffer, MAX_SIZE_NAME_DUMP, "dumps/dump.html");

    FILE *html_file = fopen(buffer, "w");
    if (!html_file) {
        printf("Error open file to dump\n");
        return ERROR_OPEN_FILE;
    }

    fprintf(html_file, "<pre>\n");

    for (int i = 1; i <= number_grpah_dump; i++) {
        fprintf(html_file, "<img src = \"dump%d.png\">\n", i);
    }

    fprintf(html_file, "</pre>\n");

    fclose(html_file);
    return ERROR_NO;
}

TypeError list_verify(List *list)
{
    assert(list);

    if (!list->data)
        return ERROR_EMPTY_POINTER;
    if (!list->next)
        return ERROR_EMPTY_POINTER;
    if (!list->prev)
        return ERROR_EMPTY_POINTER;

    if (list->size < 0)
        return ERROR_LIST_SIZE;
    if (list->capacity < 0)
        return ERROR_LIST_CAPACITY;
    if (list->capacity < list->size)
        return ERROR_LIST_CAPACITY_LESS_SIZE;

    int *color = (int *)calloc(list->capacity, sizeof(int));
    if (!color)
        return ERROR_ALLOC_FAIL;

    int ind = 0;
    int count = 0;
    while (list->next[ind] != 0) {
        color[ind] = 1;
        if (color[list->next[ind]])
            return ERROR_LIST_LOOP;

        ind = list->next[ind];
        count++;
    }
    if (count != list->size)
        return ERROR_VALID_COUNT;

    for (int i = 1; i < list->capacity; i++)
    {
        if (list->next[i] < 0 || list->next[i] > list->capacity)
            return ERROR_INVALID_INDEX_NEXT;
    }

    for (int i = 1; i < list->capacity; i++)
    {
        if (list->prev[i] < -2 || list->prev[i] > list->capacity)
            return ERROR_INVALID_INDEX_PREV;
    }

    if (list->free < 0 || list->free > list->capacity)
        return ERROR_INVALID_INDEX_FRE;

    return ERROR_NO;
}

TypeError list_dtor(List *list)
{
    if (!list)
        return ERROR_INVALID_LIST;

    TypeError err = list_verify(list);

    list->free = POISON_INT;
    list->capacity = POISON_INT;
    list->size = POISON_INT;
    free(list->data);
    free(list->next);
    free(list->prev);
    list->data = nullptr;
    list->next = nullptr;
    list->prev = nullptr;

    return err;
}

static TypeError list_increase(List *list)
{
    assert(list);

    TypeError err = list_verify(list);
    if (err)
        return err;

    int old_capacity = list->capacity;
    list->capacity = (int)(list->capacity * MULTIPLIER_CAPACITY);
    int new_capacity = list->capacity;

    list->data = (type_el *)realloc(list->data, list->capacity * sizeof(type_el));
    if (!list->data)
        return ERROR_ALLOC_FAIL;
    list->data += old_capacity;
    set_poison(list->data, new_capacity - old_capacity);
    list->data -= old_capacity;

    list->next = (int *)realloc(list->next, list->capacity * sizeof(int));
    if (!list->next)
        return ERROR_ALLOC_FAIL;
    list->next += old_capacity;
    set_index_next(list->next, old_capacity, new_capacity - old_capacity);
    list->next -= old_capacity;

    list->prev = (int *)realloc(list->prev, list->capacity * sizeof(int));
    if (!list->prev)
        return ERROR_ALLOC_FAIL;
    list->prev += old_capacity;
    set_index_prev(list->prev, new_capacity - old_capacity);
    list->prev -= old_capacity;

    if (list->free == 0)
        list->free = list->size + 1;

    return list_verify(list);
}

static TypeError list_reduce(List *list, List *new_list, int *index)
{
    assert(list);

    TypeError err = ERROR_NO;
    list_ctor(new_list);

    new_list->capacity = (int)(list->capacity / MULTIPLIER_CAPACITY);
    new_list->size = list->size;
    new_list->free = new_list->capacity;

    new_list->data = (type_el *)calloc(new_list->capacity + 1, sizeof(type_el));
    if (!new_list->data)
        return ERROR_ALLOC_FAIL;

    new_list->next = (int *)calloc(new_list->capacity + 1, sizeof(int));
    if (!new_list->next)
        return ERROR_ALLOC_FAIL;

    new_list->prev = (int *)calloc(new_list->capacity + 1, sizeof(int));
    if (!new_list->prev)
        return ERROR_ALLOC_FAIL;

    copy_valid(list, new_list, index);
    err = list_verify(list);
    if (err)
        return err;

    return list_verify(new_list);
}

TypeError list_insert(List *list, int index, type_el value, int *new_index)
{
    if (!list)
        return ERROR_INVALID_LIST;

    TypeError err = list_verify(list);
    if (err)
        return err;

    if (index < 0 || index > list->capacity || list->prev[index] == -1)
        return ERROR_INVALID_INDEX_NEW_EL;

    if (list->size >= list->capacity - 1)
        list_increase(list);

    list->size++;

    memcpy(&list->data[list->free], &value, sizeof(type_el));
    int old_fre = list->free;
    list->free = list->next[old_fre];

    list->next[old_fre] = list->next[index];
    list->prev[old_fre] = index;

    list->prev[list->next[index]] = old_fre;
    list->next[index] = old_fre;

    *new_index = old_fre;

    return list_verify(list);
}

TypeError list_erase(List *list, int index, type_el *value)
{
    if (!list)
        return ERROR_INVALID_LIST;

    TypeError err = list_verify(list);
    if (err)
        return err;

    if (list->size < list->capacity / 2)
    {
        List new_list = {};
        err = list_reduce(list, &new_list, &index);
        if (err)
            return err;

        err = list_dtor(list);
        if (err)
            return err;

        *list = new_list;
        err = list_verify(list);
        if (err)
            return err;
    }

    list->size--;
        
    index = list->next[index];

    if (index <= 0 || index >= list->capacity || list->prev[index] == -1)
        return ERROR_INVALID_INDEX_DEL_EL;

    memcpy(&value, &list->data[index], sizeof(type_el));
    memcpy(&list->data[index], &POISON_EL, sizeof(type_el));

    list->next[list->prev[index]] = list->next[index];
    list->prev[list->next[index]] = list->prev[index];

    int old_fre = list->free;
    list->free = index;
    list->next[list->free] = old_fre % list->capacity;
    list->prev[list->free] = -1;

    return list_verify(list);
}

TypeError list_get(List *list, int index, type_el *value)
{
   if (!list)
        return ERROR_INVALID_LIST;

    TypeError err = list_verify(list);
    if (err)
        return err;

    if (index < 1 || index > list->capacity)
        return ERROR_INVALID_INDEX_NEW_EL;

    memcpy(value, &list->data[index], sizeof(type_el));

    return list_verify(list);
}

TypeError list_get_index(List *list, type_el value, int *index)
{
    if (!list)
        return ERROR_INVALID_LIST;

    TypeError err = list_verify(list);
    if (err)
        return err;

    int ind = list->next[0];
    *index = -1;

    while (ind != 0) {

        if (el_cmp(&list->data[ind], &value)) {
            *index = ind;
            break;
        }

        ind = list->next[ind];
    }

    return list_verify(list);
}

static void copy_valid(List *list, List *new_list, int *index)
{
    memcpy(&new_list->data[0], &POISON_EL, sizeof(type_el));
    new_list->next[0] = 1;
    new_list->prev[0] = list->prev[0];
    int i = list->next[0], j = 1;

    while (!el_cmp(&list->data[i], &POISON_EL))
    {
        if ((*index) == i)
            *index = j;

        memcpy(&new_list->data[j], &list->data[i], sizeof(type_el));
        new_list->next[j] = (j + 1) % new_list->capacity;
        new_list->prev[j] = MAX(j - 1, -1);
        new_list->prev[0] = j;
        j++;
        i = list->next[i];
    }
}

static void set_poison(type_el *a, int size)
{
    assert(a);
    for (int i = 0; i < size; i++)
        memcpy(&a[i], &POISON_EL, sizeof(type_el));
}

static void set_index_next(int *a, int st, int size)
{
    assert(a);
    for (int i = 0; i < size; i++)
        a[i] = (i + st + 1) % (st + size);
}

static void set_index_prev(int *a, int size)
{
    assert(a);
    for (int i = 0; i < size; i++)
        a[i] = -1;
}

TypeError list_push_back(List *list, type_el value, int *new_index)
{
    return list_insert(list, list->prev[0], value, new_index);
}

TypeError list_push_front(List *list, type_el value, int *new_index)
{
    return list_insert(list, 0, value, new_index);
}

TypeError list_erase_back(List *list, type_el *value)
{
    return list_erase(list, list->prev[list->prev[0]], value);
}

TypeError list_erase_front(List *list, type_el *value)
{
    return list_erase(list, 0, value);
}