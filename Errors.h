#ifndef ERRORS_H
#define ERRORS_H

struct Error {
    int error;
    const char *description;
};

enum Type_Error {
    ERROR_NO =                          0,
    ERROR_ALLOC_FAIL =                  1 << 0,
    ERROR_MEMCPY_FAIL =                 1 << 1,
    ERROR_EMPTY_POINTER =               1 << 2,
    ERROR_DTOR_EMPTY_LIST =             1 << 3,
    ERROR_LIST_CAPACITY =               1 << 4,
    ERROR_LIST_SIZE =                   1 << 5,
    ERROR_LIST_CAPACITY_LESS_SIZE =     1 << 6,
    ERROR_INVALID_INDEX_NEXT =          1 << 7,
    ERROR_INVALID_INDEX_PREV =          1 << 8,
    ERROR_INVALID_INDEX_FRE =           1 << 9,
    ERROR_INVALID_INDEX_NEW_EL =        1 << 10,
    ERROR_INVALID_INDEX_DEL_EL =        1 << 11,
    ERROR_INVALID_LIST =                1 << 12,
    ERROR_INVALID_FILE =                1 << 13,
};

const Error ERRORS[] = {
    {ERROR_NO,                          "no errors"},
    {ERROR_ALLOC_FAIL,                  "realloc fail"},
    {ERROR_MEMCPY_FAIL,                 "memcpy fail"},
    {ERROR_EMPTY_POINTER,               "free NULL"},
    {ERROR_DTOR_EMPTY_LIST,             "try to dtor empty list"},
    {ERROR_LIST_CAPACITY,               "error in list capacity"},
    {ERROR_LIST_SIZE,                   "error in list size"},
    {ERROR_LIST_CAPACITY_LESS_SIZE,     "list capacity less than size"},
    {ERROR_INVALID_INDEX_NEXT,          "invalid index next in array"},
    {ERROR_INVALID_INDEX_PREV,          "invalid index prev in array"},
    {ERROR_INVALID_INDEX_FRE,           "invalid index fre in array"},
    {ERROR_INVALID_INDEX_NEW_EL,        "invalid index new element in array"},
    {ERROR_INVALID_INDEX_DEL_EL,        "invalid index delete element in array"},
    {ERROR_INVALID_LIST,                "invalid pointer to list"},
    {ERROR_INVALID_FILE,                "invalid name of file"},
};
const int COUNT_ERRORS = sizeof(ERRORS) / sizeof(Error);

#define err_dump(err) err_dump_(err, __FILE__, __PRETTY_FUNCTION__, __LINE__) 
void err_dump_(int err, const char *file, const char *func, int line);

#endif // ERRORS_H