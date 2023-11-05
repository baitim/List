#include <stdio.h>

#include "ANSI_colors.h"
#include "List.h"

int main()
{
    printf(print_lblue("# Implementation of List.\n"
                       "# (c) BAIDUSENOV TIMUR, 2023\n\n"));
    
    int err = 0;

    List list = {};

    err = list_ctor(&list);
    if (err) {
        err_dump(err);
        return err;
    }

    err = list_dump(&list);
    if (err) {
        err_dump(err);
        return err;
    }

    for (int i = 1; i <= 10; i++) {
        int new_index = 0;
        err = list_insert(&list, i, i + 10, &new_index);
        if (err) {
            err_dump(err);
            return err;
        }
        err = list_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 10; i > 6; i--) {
        if ((i) % 3 == 2) continue;
        int x = 0;
        err = list_erase(&list, i, &x);
        if (err) {
            err_dump(err);
            return err;
        }
        printf("POPED ELEMENT = %d\n", x);
        err = list_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 3; i <= 6; i+=2) {
        int new_index = 0;
        err = list_insert(&list, i, i + 50, &new_index);
        if (err) {
            err_dump(err);
            return err;
        }
        err = list_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    err = list_dtor(&list);
    if (err) {
        err_dump(err);
        return err;
    }

    printf(print_lblue("\nBye\n"));

    return 0;
}