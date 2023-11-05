#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"

const int DEFAULT_SIZE = 0;
const int DEFAULT_CAPACITY = 4;
const double MULTIPLIER_CAPACITY = 2;
const int POISON_EL = -0xbe;

static Type_Error list_increase(List *list);
static Type_Error list_reduce  (List *list, List *new_list);
static void copy_valid(List *list, List *new_list);
static void set_poison(int **a, int size);
static void set_index_next(int **a, int st, int size);
static void set_index_prev(int **a, int st, int size);

Type_Error list_ctor(List *list)
{
    assert(list);

    list->fre = 1;
    list->capacity = DEFAULT_CAPACITY;
    list->size = DEFAULT_SIZE;

    list->data = (int *) calloc(list->capacity, sizeof(int));
    if (!list->data) 
        return ERROR_ALLOC_FAIL;
    set_poison(&list->data, list->capacity);

    list->next = (int *) calloc(list->capacity, sizeof(int));
    if (!list->next) 
        return ERROR_ALLOC_FAIL;
    set_index_next(&list->next, 0, list->capacity);

    list->prev = (int *) calloc(list->capacity, sizeof(int));
    if (!list->prev) 
        return ERROR_ALLOC_FAIL;
    set_index_prev(&list->prev, 0, list->capacity);

    return list_verify(list);
}

Type_Error list_dump(List *list)
{
    assert(list);

    Type_Error err = list_verify(list);
    if (err) return err;

    for (int i = 0; i < list->capacity * (int)sizeof(double); i++)
        printf("v");
    printf("\nLIST:\n\n");

    printf("FRE = %d\n", list->fre);
    printf("SIZ = %d\n", list->size);
    printf("CAP = %d\n", list->capacity);

    for (int i = 0; i < list->capacity; i++) {
        printf("%d\t", i);
    }
    printf("\n");
    for (int i = 0; i < list->capacity; i++) {
        printf("%d\t", list->data[i]);
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

Type_Error list_verify(List *list)
{
    assert(list);

    if (!list->data) return ERROR_EMPTY_POINTER;
    if (!list->next) return ERROR_EMPTY_POINTER;
    if (!list->prev) return ERROR_EMPTY_POINTER;
    
    if (list->size < 0) 
        return ERROR_LIST_SIZE;
    if (list->capacity < 0) 
        return ERROR_LIST_CAPACITY;
    if (list->capacity < list->size) 
        return ERROR_LIST_CAPACITY_LESS_SIZE;

    for (int i = 1; i < list->capacity; i++) {
        if (list->next[i] < 0 || list->next[i] > list->capacity) {
            printf("%d\n", list->next[i]);
            return ERROR_INVALID_INDEX;
        }
    }

    for (int i = 1; i < list->capacity; i++) {
        if (list->prev[i] < 0 || list->prev[i] >= list->capacity) 
            return ERROR_INVALID_INDEX;
    }

    if (list->fre < 0 || list->fre > list->capacity)
        return ERROR_INVALID_INDEX;

    return ERROR_NO;
}

Type_Error list_dtor(List *list)
{
    assert(list);

    int is_empty = list_verify(list);

    list->fre = POISON_EL;
    list->capacity = POISON_EL;
    list->size = POISON_EL;
    free(list->data);
    free(list->next);
    free(list->prev);

    if (is_empty) return ERROR_DTOR_EMPTY_LIST;
    return ERROR_NO;
}

static Type_Error list_increase(List *list)
{
    assert(list);

    Type_Error err = list_verify(list);
    if (err) return err;

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
    set_index_prev(&list->prev, old_capacity, new_capacity - old_capacity);
    list->prev -= old_capacity;

    return list_verify(list);
}

static Type_Error list_reduce(List *list, List *new_list)
{
    assert(list);

    Type_Error err = ERROR_NO;
    list_ctor(new_list);

    new_list->capacity = (int)(list->capacity / MULTIPLIER_CAPACITY);
    new_list->size = list->size;
    new_list->fre = new_list->capacity;

    int *tmp = (int *) calloc(new_list->capacity + 1, sizeof(int));
    if (tmp != NULL)
        new_list->data = tmp;
    else
        return ERROR_ALLOC_FAIL;

    tmp = (int *) calloc(new_list->capacity + 1, sizeof(int));
    if (tmp != NULL)
        new_list->next = tmp;
    else
        return ERROR_ALLOC_FAIL;

    tmp = (int *) calloc(new_list->capacity + 1, sizeof(int));
    if (tmp != NULL)
        new_list->prev = tmp;
    else
        return ERROR_ALLOC_FAIL;

    copy_valid(list, new_list);
    err = list_verify(list);
    if (err) return err;
    return list_verify(new_list);
}

Type_Error list_insert(List *list, int index, int value, int *new_index)
{
    assert(list);

    Type_Error err = list_verify(list);
    if (err) return err;

    if (list->size + 1 >= list->capacity)
        list_increase(list);

    list->data[list->fre] = value;

    list->next[list->fre] = list->next[index];
    list->prev[list->fre] = list->prev[index];

    list->prev[list->next[list->fre]] = index;
    list->next[list->prev[list->fre]] = index;

    if (index > list->prev[0])
        list->prev[0] = index;

    if (index < list->next[0])
        list->next[0] = index;

    *new_index = list->fre;

    list->fre = list->next[list->fre];

    list->size++;

    return list_verify(list);
}

Type_Error list_erase(List *list, int index, int *value)
{
    assert(list);

    Type_Error err = list_verify(list);
    if (err) return err;

    if (list->size < list->capacity / 2 && list->capacity > DEFAULT_CAPACITY) {
        List new_list = {};
        err = list_reduce(list, &new_list);
        if (err) return err;

        err = list_dtor(list);
        if (err) return err;

        *list = new_list;
        err = list_verify(list);
        if (err) return err;
    }

    list->size--;

    *value = list->data[index];
    list->data[index] = POISON_EL;
    
    list->prev[list->next[index]] = list->prev[index];
    list->next[list->prev[index]] = list->next[index];

    if (index == list->next[0])
        list->next[0] = list->next[list->next[0]];

    if (index == list->prev[0])
        list->prev[0] = list->prev[list->prev[0]];

    int old_fre = list->fre;
    list->fre = index;
    list->next[list->fre] = old_fre;

    return list_verify(list);
}

Type_Error list_get_el(List *list, int index, int *value)
{
    assert(list);

    Type_Error err = list_verify(list);
    if (err) return err;

    if (index < 1 || index > list->capacity)
        return ERROR_INVALID_INDEX;

    *value = list->data[index];

    return list_verify(list);
}

static void copy_valid(List *list, List *new_list)
{
    new_list->data[0] = POISON_EL;
    new_list->next[0] = 1;
    int i = list->next[0], j = 1;

    while (i < list->capacity && list->data[i] != POISON_EL) {
        new_list->data[j] = list->data[i];
        new_list->next[j] = j + 1;
        new_list->prev[j] = j - 1;
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
        (*a)[i] = i + st + 1;
}

static void set_index_prev(int **a, int st, int size)
{
    assert(*a);
    for (int i = 0; i < size; i++) 
        (*a)[i] = i + st - 1;
}