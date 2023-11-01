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

    err = list_dtor(&list);
    if (err) {
        err_dump(err);
        return err;
    }

    printf(print_lblue("\nBye\n"));

    return 0;
}