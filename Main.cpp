#include <stdio.h>

#include "ANSI_colors.h"
#include "List.h"

int main()
{
    printf(print_lblue("# Implementation of List.\n"
                       "# (c) BAIDUSENOV TIMUR, 2023\n\n"));
    
    int err = 0;

    LIST list = {};

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

    for (int i = 0; i < 10; i++) {
        err = list_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
        err = list_push(&list, i, i + 10);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 0; i < 10; i++) {
        err = list_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
        int x = 0;
        err = list_pop(&list, i, &x);
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

    err = list_dtor(&list);
    if (err) {
        err_dump(err);
        return err;
    }

    printf(print_lblue("\nBye\n"));

    return 0;
}