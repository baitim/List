#include <stdio.h>

#include "ANSI_colors.h"
#include "List.h"

int main()
{
    printf(print_lblue("# Implementation of List.\n"
                       "# (c) BAIDUSENOV TIMUR, 2023\n\n"));
    
    TypeError err = ERROR_NO;
    int number_graph_dump = 1;

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
        err = list_insert(&list, i, {"che", i}, &new_index);
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
            type_el value_el = {"che", i + 10};
            err = list_get(&list, i - 1, &value_el);
            if (err) {
                err_dump(err);
                return err;
            }
            printf("Element in %d = \n", i - 1);
            print_el(&value_el);
        }
        err = list_graph_dump(&list, &number_graph_dump);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 10; i > 6; i--) {
        if ((i) % 3 == 2) continue;
        type_el x = {};
        err = list_erase(&list, i, &x);
        if (err) {
            err_dump(err);
            return err;
        }
        printf("POPED ELEMENT = \n");
        print_el(&x);
        err = list_cmd_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 2; i >= 0; i--) {
        type_el x = {};
        err = list_erase(&list, i, &x);
        if (err) {
            err_dump(err);
            return err;
        }
        printf("POPED ELEMENT = \n");
        print_el(&x);
        err = list_cmd_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 4; i >= 3; i--) {
        type_el x = {};
        err = list_erase(&list, i, &x);
        if (err) {
            err_dump(err);
            return err;
        }
        printf("POPED ELEMENT = \n");
        print_el(&x);
        err = list_cmd_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 0; i <= 5; i++) {
        if ((i) % 3 == 1) continue;
        int x = 0;
        err = list_push_front(&list, {"angem", i + 20}, &x);
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
        err = list_push_back(&list, {"discra", i + 30}, &x);
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
            type_el x = {};
            err = list_erase_front(&list, &x);
            if (err) {
                err_dump(err);
                return err;
            }
            printf("FRONT POPED ELEMENT = \n");
            print_el(&x);
            err = list_cmd_dump(&list);
            if (err) {
                err_dump(err);
                return err;
            }
        } else {
            type_el x = {};
            err = list_erase_back(&list, &x);
            if (err) {
                err_dump(err);
                return err;
            }
            printf("BACK POPED ELEMENT = \n");
            print_el(&x);
            err = list_cmd_dump(&list);
            if (err) {
                err_dump(err);
                return err;
            }
        }
    }

    for (int i = 0; i < 6; i++) {
        type_el x = {};
        err = list_erase_back(&list, &x);
        if (err) {
            err_dump(err);
            return err;
        }
        printf("BACK POPED ELEMENT = \n");
        print_el(&x);
        err = list_cmd_dump(&list);
        if (err) {
            err_dump(err);
            return err;
        }
    }

    for (int i = 0; i <= 3; i++) {
        if ((i) % 2 == 0) {
            int x = 0;
            err = list_push_front(&list, {"obchesos", i + 40}, &x);
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
            err = list_push_back(&list, {"proga", i + 50}, &x);
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

    err = list_graph_dump(&list, &number_graph_dump);
    if (err) {
        err_dump(err);
        return err;
    }

    err = list_dtor(&list);
    if (err) {
        err_dump(err);
        return err;
    }

    err = list_html_dump(number_graph_dump);
    if (err) {
        err_dump(err);
        return err;
    }

    printf(print_lblue("\nBye\n"));

    return 0;
}