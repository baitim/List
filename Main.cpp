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

    err = list_cmd_dump(&list);
    if (err) {
        err_dump(err);
        return err;
    }

    for (int i = 0; i <= 10; i++) {
        int new_index = 0;
        err = list_insert(&list, i, i + 10, &new_index);
        if (err) {
            err_dump(err);
            return err;
        }
        err = list_cmd_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
        if (i % 3 == 2) {
            int value_el = 0;
            err = list_get(&list, i - 1, &value_el);
            if (err) {
                err_dump(err);
                return err;
            }
            printf("Element in %d = %d\n", i - 1, value_el);
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
        err = list_cmd_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    int value_el = 0;
    err = list_get(&list, 7, &value_el);
    if (err) {
        err_dump(err);
        return err;
        }
    printf("Element in %d = %d\n", 7, value_el);

    value_el = 0;
    err = list_get(&list, 9, &value_el);
    if (err) {
        err_dump(err);
        return err;
        }
    printf("Element in %d = %d\n", 9, value_el);

    for (int i = 2; i >= 0; i--) {
        int x = 0;
        err = list_erase(&list, i, &x);
        if (err) {
            err_dump(err);
            return err;
        }
        printf("POPED ELEMENT = %d\n", x);
        err = list_cmd_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 4; i >= 3; i--) {
        int x = 0;
        err = list_erase(&list, i, &x);
        if (err) {
            err_dump(err);
            return err;
        }
        printf("POPED ELEMENT = %d\n", x);
        err = list_cmd_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 0; i <= 5; i++) {
        if ((i) % 3 == 1) continue;
        int x = 0;
        err = push_front(&list, i + 20, &x);
        if (err) {
            err_dump(err);
            return err;
        }
        printf("FRONT NEW INDEX = %d\n", x);
        err = list_cmd_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 0; i <= 4; i++) {
        if ((i) % 3 == 1) continue;
        int x = 0;
        err = push_back(&list, i + 30, &x);
        if (err) {
            err_dump(err);
            return err;
        }
        printf("BACK NEW INDEX = %d\n", x);
        err = list_cmd_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 0; i < 4; i++) {
        printf("\ni = %d\n", i);
        if ((i) % 2 == 0) {
            int x = 0;
            err = erase_front(&list, &x);
            if (err) {
                err_dump(err);
                return err;
            }
            printf("FRONT POPED ELEMENT = %d\n", x);
            err = list_cmd_dump(&list);
            if (err) {
                err_dump(err);
                return err;
            }
        } else {
            int x = 0;
            err = erase_back(&list, &x);
            if (err) {
                err_dump(err);
                return err;
            }
            printf("BACK POPED ELEMENT = %d\n", x);
            err = list_cmd_dump(&list);
            if (err) {
                err_dump(err);
                return err;
            }
        }
    }

    for (int i = 0; i < 6; i++) {
        int x = 0;
        err = erase_back(&list, &x);
        if (err) {
            err_dump(err);
            return err;
        }
        printf("BACK POPED ELEMENT = %d\n", x);
        err = list_cmd_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 0; i <= 3; i++) {
        if ((i) % 2 == 0) {
            int x = 0;
            err = push_front(&list, i + 60, &x);
            if (err) {
                err_dump(err);
                return err;
            }
            printf("FRONT NEW INDEX = %d\n", x);
            err = list_cmd_dump(&list);
            if (err) {
                err_dump(err);
                return err;
            }
        } else {
            int x = 0;
            err = push_back(&list, i + 70, &x);
            if (err) {
                err_dump(err);
                return err;
            }
            printf("BACK NEW INDEX = %d\n", x);
            err = list_cmd_dump(&list);
            if (err) {
                err_dump(err);
                return err;
            }
        }
    }

    int x = 60;
    int ind = 0;
    err = list_get_index(&list, x, &ind);
    if (err) {
        err_dump(err);
        return err;
    }
    printf("INDEX element(%d) = %d\n", x, ind);

    FILE *dump_file = fopen("dump.dot", "w");
    if (!dump_file) {
        printf("Error open file to dump\n");
        return 1;
    }

    list_graph_dump(&list, dump_file);

    fclose(dump_file);

    err = list_dtor(&list);
    if (err) {
        err_dump(err);
        return err;
    }

    printf(print_lblue("\nBye\n"));

    return 0;
}