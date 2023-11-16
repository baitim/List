#include <assert.h>
#include <stdio.h>
#include <cstring>

#include "Config.h"
#include "ANSI_colors.h"

void print_el(const type_el* elem)
{
    printf("%s %d\t", elem->name, elem->years);
}

void print_dump_el(const type_el* elem, FILE *file)
{
    assert(file);

    fprintf(file, "%s %d\t", elem->name, elem->years);
}

bool el_cmp(const type_el* elem, const type_el *obj)
{
    return memcmp(elem, obj, sizeof(type_el)) == 0;
}