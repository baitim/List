#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "List.h"

static void set_poison(int **a, int size);

TypeError list_ctor(LIST *list)
{
    assert(list);

    list->capacity = MAX_SIZE_LIST;
    list->size = 0;

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

    return ERROR_NO;
}

TypeError list_dump(LIST *list)
{
    assert(list);

    TypeError err = list_verify(list);
    if (err) return err; 

    for (int i = 0; i < 10; i++) {
        printf("%d\t", i + 1);
    }
    printf("\n");
    for (int i = 0; i < 10; i++) {
        printf("%d\t", list->data[i]);
    }
    printf("\n");
    for (int i = 0; i < 10; i++) {
        printf("%d\t", list->next[i]);
    }
    printf("\n");
    for (int i = 0; i < 10; i++) {
        printf("%d\t", list->prev[i]);
    }
    printf("\n");

    return list_verify(list);
}

TypeError list_verify(LIST *list)
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

TypeError list_dtor(LIST *list)
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

static void set_poison(int **a, int size)
{
    assert(*a);
    for (int i = 0; i < size; i++) 
        (*a)[i] = POISON_EL;
}