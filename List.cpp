#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"

const int DEFAULT_SIZE = 0;
const int DEFAULT_CAPACITY = 4;
const double MULTIPLIER_CAPACITY = 2;
const int POISON_EL = -0xbe;

static TYPE_ERROR list_increase(LIST *list);
static TYPE_ERROR list_reduce  (LIST *list, LIST **new_list);
static void copy_valid(int **dest, int *src, int size_src);
static void set_poison(int **a, int size);

TYPE_ERROR list_ctor(LIST *list)
{
    assert(list);

    list->capacity = DEFAULT_CAPACITY;
    list->size = DEFAULT_SIZE;

    list->data = (int *) calloc(list->capacity, sizeof(int));
    if (!list->data) 
        return ERROR_ALLOC_FAIL;
    set_poison(&list->data, list->capacity);

    list->next = (int *) calloc(list->capacity, sizeof(int));
    if (!list->next) 
        return ERROR_ALLOC_FAIL;
    set_poison(&list->next, list->capacity);

    list->prev = (int *) calloc(list->capacity, sizeof(int));
    if (!list->prev) 
        return ERROR_ALLOC_FAIL;
    set_poison(&list->prev, list->capacity);

    return list_verify(list);
}

TYPE_ERROR list_dump(LIST *list)
{
    assert(list);

    TYPE_ERROR err = list_verify(list);
    if (err) return err;

    for (int i = 0; i < list->capacity * (int)sizeof(double); i++)
        printf("v");
    printf("\nLIST:\n\n");

    for (int i = 0; i < list->capacity; i++) {
        printf("%d\t", i + 1);
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

TYPE_ERROR list_verify(LIST *list)
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

    for (int i = 0; i < list->capacity; i++) {
        if ((list->next[i] != POISON_EL && list->next[i] < 0) 
            || list->next[i] >= list->capacity) 
            return ERROR_INVALID_INDEX;
    }
    for (int i = 0; i < list->capacity; i++) {
        if ((list->prev[i] != POISON_EL && list->prev[i] < 0) 
            || list->prev[i] >= list->capacity) 
            return ERROR_INVALID_INDEX;
    }

    return ERROR_NO;
}

TYPE_ERROR list_dtor(LIST *list)
{
    assert(list);

    int is_empty = list_verify(list);

    list->capacity = POISON_EL;
    list->size = POISON_EL;
    free(list->data);
    free(list->next);
    free(list->prev);

    if (is_empty) return ERROR_DTOR_EMPTY_LIST;
    return ERROR_NO;
}

static TYPE_ERROR list_increase(LIST *list)
{
    assert(list);

    TYPE_ERROR err = list_verify(list);
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
    set_poison(&list->next, new_capacity - old_capacity);
    list->next -= old_capacity;

    list->prev = (int *)realloc(list->prev, list->capacity * sizeof(int));
    if (!list->prev)
        return ERROR_ALLOC_FAIL;
    list->prev += old_capacity;
    set_poison(&list->prev, new_capacity - old_capacity);
    list->prev -= old_capacity;

    return list_verify(list);
}

static TYPE_ERROR list_reduce(LIST *list, LIST **new_list)
{
    assert(list);

    TYPE_ERROR err = list_verify(list);
    if (err) return err;

    *new_list = (LIST *) malloc(sizeof(LIST));
    err = list_ctor((*new_list));
    if (err) return err;

    (*new_list)->capacity = (int)(list->capacity / MULTIPLIER_CAPACITY);
    (*new_list)->size = list->size;

    (*new_list)->data = (int *) realloc((*new_list)->data, (*new_list)->capacity * sizeof(int));
    if (!(*new_list)->data) 
        return ERROR_ALLOC_FAIL;
    copy_valid(&(*new_list)->data, list->data, list->capacity);

    (*new_list)->next = (int *) realloc((*new_list)->next, (*new_list)->capacity * sizeof(int));
    if (!(*new_list)->next) 
        return ERROR_ALLOC_FAIL;
    copy_valid(&(*new_list)->next, list->next, list->capacity);

    (*new_list)->prev = (int *) realloc((*new_list)->prev, (*new_list)->capacity * sizeof(int));
    if (!(*new_list)->prev) 
        return ERROR_ALLOC_FAIL;
    copy_valid(&(*new_list)->prev, list->prev, list->capacity);

    err = list_verify(list);
    if (err) return err;
    return list_verify((*new_list));
}

TYPE_ERROR list_push(LIST *list, int index, int value)
{
    assert(list);

    TYPE_ERROR err = list_verify(list);
    if (err) return err;

    if (list->size >= list->capacity)
        list_increase(list);

    list->data[list->size] = value;
    list->next[list->size] = list->size;
    list->prev[list->size] = list->size;

    list->size++;

    return list_verify(list);
}

TYPE_ERROR list_pop(LIST *list, int index, int *value)
{
    assert(list);

    TYPE_ERROR err = list_verify(list);
    if (err) return err;

    if (list->size <= list->capacity / 2 && list->capacity > DEFAULT_CAPACITY) {
        LIST *new_list = NULL;
        err = list_reduce(list, &new_list);
        if (err) return err;
        err = list_dtor(list);

        if (err) return err;
        *list = *new_list;

        err = list_verify(list);
        if (err) return err;
    }

    list->size--;

    *value = list->data[list->size];
    list->data[list->size] = POISON_EL;
    list->next[list->size] = POISON_EL;
    list->prev[list->size] = POISON_EL;

    return list_verify(list);
}

static void copy_valid(int **dest, int *src, int size_src)
{
    int j = 0;
    for (int i = 0; i < size_src; i++) {
        if (src[i] != POISON_EL) {
            (*dest)[j] = src[i];
            j++;
        }
    }
}

static void set_poison(int **a, int size)
{
    assert(*a);
    for (int i = 0; i < size; i++) 
        (*a)[i] = POISON_EL;
}