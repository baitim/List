#ifndef ERRORS_H
#define ERRORS_H

struct ERROR {
    int error;
    const char *description;
};

enum TYPE_ERROR {
    ERROR_NO =                          0,
    ERROR_ALLOC_FAIL =                  1 << 0,
    ERROR_MEMCPY_FAIL =                 1 << 1,
    ERROR_EMPTY_POINTER =               1 << 2,
    ERROR_DTOR_EMPTY_LIST =             1 << 3,
    ERROR_LIST_CAPACITY =               1 << 4,
    ERROR_LIST_SIZE =                   1 << 5,
    ERROR_LIST_CAPACITY_LESS_SIZE =     1 << 6,
    ERROR_INVALID_INDEX =               1 << 7,
};

const ERROR ERRORS[] = {
    {ERROR_NO,                          "no errors"},
    {ERROR_ALLOC_FAIL,                  "realloc fail"},
    {ERROR_MEMCPY_FAIL,                 "memcpy fail"},
    {ERROR_EMPTY_POINTER,               "free NULL"},
    {ERROR_DTOR_EMPTY_LIST,             "try to dtor empty list"},
    {ERROR_LIST_CAPACITY,               "error in list capacity"},
    {ERROR_LIST_SIZE,                   "error in list size"},
    {ERROR_LIST_CAPACITY_LESS_SIZE,     "list capacity less than size"},
    {ERROR_INVALID_INDEX,               "invalid index in array"},
};
const int COUNT_ERRORS = sizeof(ERRORS) / sizeof(ERROR);

#define err_dump(err) err_dump_(err, __FILE__, __PRETTY_FUNCTION__, __LINE__) 
void err_dump_(int err, const char *file, const char *func, int line);

#endif // ERRORS_H