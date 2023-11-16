#ifndef CONFIG_H
#define CONFIG_H

const int MAX_SIZE_NAME = 100;

struct Client {
    char name[MAX_SIZE_NAME];
    int years;
};

typedef Client type_el;

const type_el POISON_EL = {"", -1};

void print_el(const type_el* elem);
void print_dump_el(const type_el* elem, FILE *file);
bool el_cmp(const type_el* elem, const type_el *obj);

#endif // CONFIG_H