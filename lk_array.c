#include "lk_array.h"

LkArray* lk_new_array(size_t size, size_t memb_size) {
    LkArray* arr = lk_new(LkArray);
    if (!arr)
        return NULL;

    arr->data      = LK_CALLOC(size, memb_size);
    arr->memb_size = memb_size;
    arr->size      = size;

    if (!arr->data) {
        LK_FREE(arr);
        return NULL;
    }

    return arr;
}

void lk_free_array_internal(LkArray* ptr) {
    if (!ptr) {
        return;
    }
    free(ptr->data);
    free(ptr);
}
