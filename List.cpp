#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"

#define MAX(a, b) ((a) > (b)) ? (a) : (b)

const int DEFAULT_SIZE = 0;
const int DEFAULT_CAPACITY = 1;
const double MULTIPLIER_CAPACITY = 2;
const int POISON_EL = -0xbe;

static Type_Error list_increase(List *list);
static Type_Error list_reduce(List *list, List *new_list, int *index);
static void copy_valid(List *list, List *new_list, int *index);
static void set_poison(int **a, int size);
static void set_index_next(int **a, int st, int size);
static void set_index_prev(int **a, int size);

Type_Error list_ctor(List *list)
{
    if (!list)
        return ERROR_INVALID_LIST;

    list->fre = 1;
    list->capacity = DEFAULT_CAPACITY;
    list->size = DEFAULT_SIZE;

    list->data = (int *)calloc(list->capacity, sizeof(int));
    if (!list->data)
        return ERROR_ALLOC_FAIL;
    set_poison(&list->data, list->capacity);

    list->next = (int *)calloc(list->capacity, sizeof(int));
    if (!list->next)
        return ERROR_ALLOC_FAIL;
    set_index_next(&list->next, 0, list->capacity);
    list->next[0] = 0;

    list->prev = (int *)calloc(list->capacity, sizeof(int));
    if (!list->prev)
        return ERROR_ALLOC_FAIL;
    set_index_prev(&list->prev, list->capacity);
    list->prev[0] = 0;

    return list_verify(list);
}

Type_Error list_cmd_dump(List *list)
{
    if (!list)
        return ERROR_INVALID_LIST;

    Type_Error err = list_verify(list);
    if (err)
        return err;

    for (int i = 0; i < list->capacity * (int)sizeof(double); i++)
        printf("v");
    printf("\nLIST:\n\n");

    printf("FRE = %d\n", list->fre);
    printf("SIZ = %d\n", list->size);
    printf("CAP = %d\n", list->capacity);

    for (int i = 0; i < list->capacity; i++)
    {
        printf("%d\t", i);
    }
    printf("\n");
    for (int i = 0; i < list->capacity; i++)
    {
        printf("%d\t", list->data[i]);
    }
    printf("\n");
    for (int i = 0; i < list->capacity; i++)
    {
        printf("%d\t", list->next[i]);
    }
    printf("\n");
    for (int i = 0; i < list->capacity; i++)
    {
        printf("%d\t", list->prev[i]);
    }
    printf("\n");
    for (int i = 0; i < list->capacity * (int)sizeof(double); i++)
        printf("^");
    printf("\n");

    return list_verify(list);
}

Type_Error list_graph_dump(List *list, FILE *dump_file)
{
    if (!list)
        return ERROR_INVALID_LIST;

    if (!dump_file)
        return ERROR_INVALID_FILE;

    Type_Error err = list_verify(list);
    if (err)
        return err;

    fprintf(dump_file, "digraph {\n");
    fprintf(dump_file, "\tgraph[label = \"List\", labelloc = top, ");
    fprintf(dump_file, "labeljust = center, fontsize = 70, fontcolor = \"#e33e19\"];\n");
    fprintf(dump_file, "\tgraph[dpi = 200];\n");
	fprintf(dump_file, "\tsplines = ortho;\n");
    fprintf(dump_file, "\tbgcolor = \"#2F353B\"\n");
    fprintf(dump_file, "\tedge[minlen = 3, penwidth = 2.5];\n");

    fprintf(dump_file, "\tnode[shape = \"rectangle\", style = \"rounded, filled\", height = 3, width = 2, ");
	fprintf(dump_file, "fillcolor = \"#ab5b0f\", fontsize = 30, penwidth = 3.5, color = \"#941b1b\"]\n");

    fprintf(dump_file, "\t{ rank = same;\n");
    fprintf(dump_file, "\t\tnode[shape = \"Mrecord\"];\n");
    for (int i = 1; i < list->capacity; i++) {
        fprintf(dump_file, "\t\tnode%d[label = \"{ %d | %d | %d | %d }\"",
                            i, i, list->data[i], list->next[i], list->prev[i]);
        if (list->prev[i] == -1)
            fprintf(dump_file, ", fillcolor = \"#e3964d\"");
        fprintf(dump_file, "];\n");
    }
    fprintf(dump_file, "\t}\n");

    fprintf(dump_file, "\t{ rank = min;\n");
    fprintf(dump_file, "\t\tnode[shape = \"Mrecord\"];\n");
    fprintf(dump_file, "\t\tnode%d[label = \"{ %d | %d | %d | %d }\"];\n",
                            0, 0, list->data[0], list->next[0], list->prev[0]);
    fprintf(dump_file, "\t}\n");

    fprintf(dump_file, "\t{ rank = max;\n");
    fprintf(dump_file, "\t\tnode_free[label = \"free\"];\n");
    fprintf(dump_file, "\t}\n");

    fprintf(dump_file, "\tnode_free->node%d [color = orange];\n", list->fre);

    for (int i = 0; i < list->capacity - 1; i++) {
        fprintf(dump_file, "\tnode%d->node%d[weight = 100, style = invis];\n",
                            i, i + 1);
    }

    for (int i = 0; i < list->capacity; i++) {
        fprintf(dump_file, "\tnode%d->node%d[color = yellow];\n",
                            i, list->next[i]);
    }

    fprintf(dump_file, "}\n");

    return list_verify(list);
}

Type_Error list_verify(List *list)
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

    if (list->fre < 0 || list->fre > list->capacity)
        return ERROR_INVALID_INDEX_FRE;

    return ERROR_NO;
}

Type_Error list_dtor(List *list)
{
    if (!list)
        return ERROR_INVALID_LIST;

    int is_empty = list_verify(list);

    list->fre = POISON_EL;
    list->capacity = POISON_EL;
    list->size = POISON_EL;
    free(list->data);
    free(list->next);
    free(list->prev);

    if (is_empty)
        return ERROR_DTOR_EMPTY_LIST;
    return ERROR_NO;
}

static Type_Error list_increase(List *list)
{
    assert(list);

    Type_Error err = list_verify(list);
    if (err)
        return err;

    int old_capacity = list->capacity;
    list->capacity = (int)(list->capacity * MULTIPLIER_CAPACITY);
    int new_capacity = list->capacity;

    list->data = (int *)realloc(list->data, list->capacity * sizeof(int));
    if (!list->data)
        return ERROR_ALLOC_FAIL;
    list->data += old_capacity;
    set_poison(&list->data, new_capacity - old_capacity);
    list->data -= old_capacity;

    list->next = (int *)realloc(list->next, list->capacity * sizeof(int));
    if (!list->next)
        return ERROR_ALLOC_FAIL;
    list->next += old_capacity;
    set_index_next(&list->next, old_capacity, new_capacity - old_capacity);
    list->next -= old_capacity;

    list->prev = (int *)realloc(list->prev, list->capacity * sizeof(int));
    if (!list->prev)
        return ERROR_ALLOC_FAIL;
    list->prev += old_capacity;
    set_index_prev(&list->prev, new_capacity - old_capacity);
    list->prev -= old_capacity;

    return list_verify(list);
}

static Type_Error list_reduce(List *list, List *new_list, int *index)
{
    assert(list);

    Type_Error err = ERROR_NO;
    list_ctor(new_list);

    new_list->capacity = (int)(list->capacity / MULTIPLIER_CAPACITY);
    new_list->size = list->size;
    new_list->fre = new_list->capacity;

    int *tmp = (int *)calloc(new_list->capacity + 1, sizeof(int));
    if (tmp != NULL)
        new_list->data = tmp;
    else
        return ERROR_ALLOC_FAIL;

    tmp = (int *)calloc(new_list->capacity + 1, sizeof(int));
    if (tmp != NULL)
        new_list->next = tmp;
    else
        return ERROR_ALLOC_FAIL;

    tmp = (int *)calloc(new_list->capacity + 1, sizeof(int));
    if (tmp != NULL)
        new_list->prev = tmp;
    else
        return ERROR_ALLOC_FAIL;

    copy_valid(list, new_list, index);
    err = list_verify(list);
    if (err)
        return err;

    return list_verify(new_list);
}

Type_Error list_insert(List *list, int index, int value, int *new_index)
{
    if (!list)
        return ERROR_INVALID_LIST;

    Type_Error err = list_verify(list);
    if (err)
        return err;

    if (index < 0 || index > list->capacity || list->prev[index] == -1)
        return ERROR_INVALID_INDEX_NEW_EL;

    list->size++;

    err = list_verify(list);
    if (err)
        return err;

    while (list->size >= list->capacity)
        list_increase(list);
    
    if (list->fre == 0)
        list->fre = list->size;

    list->data[list->fre] = value;
    int old_fre = list->fre;
    list->fre = list->next[old_fre];

    list->next[old_fre] = list->next[index];
    list->prev[old_fre] = index;

    list->prev[list->next[index]] = old_fre;
    list->next[index] = old_fre;

    *new_index = old_fre;

    if (index == list->prev[0]) {
        list->prev[0] = old_fre;
        list->next[old_fre] = 0;
    }

    return list_verify(list);
}

Type_Error list_erase(List *list, int index, int *value)
{
    if (!list)
        return ERROR_INVALID_LIST;

    Type_Error err = list_verify(list);
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

    err = list_verify(list);
    if (err)
        return err;
        
    index = list->next[index];

    if (index <= 0 || index >= list->capacity || list->prev[index] == -1)
        return ERROR_INVALID_INDEX_DEL_EL;

    *value = list->data[index];
    list->data[index] = POISON_EL;

    if (list->next[index] == 0) 
        list->next[list->prev[index]] = 0;
    else
        list->next[list->prev[index]] = list->next[index];

    list->prev[list->next[index]] = list->prev[index];

    if (index == list->next[0])
        list->next[0] = list->next[list->next[0]];

    if (index == list->prev[0])
        list->prev[0] = list->prev[list->prev[0]];

    int old_fre = list->fre;
    list->fre = index;
    list->next[list->fre] = old_fre % list->capacity;
    list->prev[list->fre] = -1;

    return list_verify(list);
}

Type_Error list_get(List *list, int index, int *value)
{
   if (!list)
        return ERROR_INVALID_LIST;

    Type_Error err = list_verify(list);
    if (err)
        return err;

    if (index < 1 || index > list->capacity)
        return ERROR_INVALID_INDEX_NEW_EL;

    *value = list->data[index];

    return list_verify(list);
}

Type_Error list_get_index(List *list, int value, int *index)
{
    if (!list)
        return ERROR_INVALID_LIST;

    Type_Error err = list_verify(list);
    if (err)
        return err;

    int ind = list->next[0];
    *index = -1;

    while (ind != 0) {

        if (list->data[ind] == value) {
            *index = ind;
            break;
        }

        ind = list->next[ind];
    }

    return list_verify(list);
}

static void copy_valid(List *list, List *new_list, int *index)
{
    new_list->data[0] = POISON_EL;
    new_list->next[0] = 1;
    new_list->prev[0] = list->prev[0];
    int i = list->next[0], j = 1;

    while (list->data[i] != POISON_EL)
    {
        if ((*index) == i)
            *index = j;

        new_list->data[j] = list->data[i];
        new_list->next[j] = (j + 1) % new_list->capacity;
        new_list->prev[j] = MAX(j - 1, -1);
        new_list->prev[0] = j;
        j++;
        i = list->next[i];
    }
}

static void set_poison(int **a, int size)
{
    assert(*a);
    for (int i = 0; i < size; i++)
        (*a)[i] = POISON_EL;
}

static void set_index_next(int **a, int st, int size)
{
    assert(*a);
    for (int i = 0; i < size; i++)
        (*a)[i] = (i + st + 1) % (st + size);
}

static void set_index_prev(int **a, int size)
{
    assert(*a);
    for (int i = 0; i < size; i++)
        (*a)[i] = -1;
}

Type_Error push_back(List *list, int value, int *new_index)
{
    if (!list)
        return ERROR_INVALID_LIST;

    Type_Error err = list_verify(list);
    if (err)
        return err;

    err = list_insert(list, list->prev[0], value, new_index);
    if (err)
        return err;

    return list_verify(list);
}

Type_Error push_front(List *list, int value, int *new_index)
{
    if (!list)
        return ERROR_INVALID_LIST;

    Type_Error err = list_verify(list);
    if (err)
        return err;

    err = list_insert(list, 0, value, new_index);
    if (err)
        return err;

    return list_verify(list);
}

Type_Error erase_back(List *list, int *value)
{
    if (!list)
        return ERROR_INVALID_LIST;

    Type_Error err = list_verify(list);
    if (err)
        return err;

    if (list->prev[0] == 0)
        return ERROR_INVALID_INDEX_DEL_EL;

    err = list_erase(list, list->prev[list->prev[0]], value);
    if (err)
        return err;

    return list_verify(list);
}

Type_Error erase_front(List *list, int *value)
{
    if (!list)
        return ERROR_INVALID_LIST;

    Type_Error err = list_verify(list);
    if (err)
        return err;

    if (list->next[0] == 0)
        return ERROR_INVALID_INDEX_DEL_EL;

    err = list_erase(list, 0, value);
    if (err)
        return err;

    return list_verify(list);
}