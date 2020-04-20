#include "lk_array.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

static callback_ptr error_callback = NULL;

// FIXME: This is very slow and ugly.
#define report_error(str)              \
    do {                               \
        if (error_callback) {          \
            char buf[512];             \
            strcpy(buf, __FUNCTION__); \
            strcat(buf, ": ");         \
            strcat(buf, str);          \
            error_callback(buf);       \
        }                              \
    } while (0)

void lk_setup_error_callback(callback_ptr fn) {
    error_callback = fn;
}

static void stderr_handler(const char* msg) {
    if (msg)
        fprintf(stderr, "%s\n", msg);
}

void lk_setup_error_callback_stderr() {
    error_callback = stderr_handler;
}

lk_array* lk_new_array(size_t size, size_t memb_size) {
    if (memb_size == 0) {
        report_error("memb_size may never be 0");
        return NULL;
    }

    lk_array* arr = lk_new(lk_array);
    if (!arr) {
        report_error("LK_MALLOC failed");
        return NULL;
    }

    if (size == 0) {
        arr->data      = NULL;
        arr->memb_size = memb_size;
        arr->size      = 0;
        arr->capacity  = 0;
    } else {
        arr->data = LK_CALLOC(size, memb_size);
        if (!arr->data) {
            LK_FREE(arr);
            report_error("LK_CALLOC failed");
            return NULL;
        }
        arr->memb_size = memb_size;
        arr->size      = size;
        arr->capacity  = size;
    }

    return arr;
}

void lk_free_array_internal(lk_array* ptr) {
    if (!ptr) {
        // freeing a NULL ptr is okay, no error
        return;
    }
    free(ptr->data);
    free(ptr);
}

bool lk_array_deep_copy(lk_array* dest, lk_array* src) {
    if (!src) {
        report_error("source cannot be NULL");
        return false;
    }
    if (!dest) {
        report_error("dest cannot be NULL");
        return false;
    }


    // reallocarray to save us free'ing and calloc'ing here if
    // the arrays are the same size or src->size < dest->size
    void* new_data = LK_REALLOCARRAY(dest->data, src->size, src->memb_size);
    if (!new_data) {
        report_error("LK_REALLOCARRAY failed");
        return false;
    }

    // realloc was successful, set ptr
    dest->data = new_data;
    // copy size
    dest->size = src->size;
    // capacity is size for dest, since src might have different capacity
    dest->capacity = dest->size;
    // copy member size
    dest->memb_size = src->memb_size;

    if (!src->data) {
        report_error("src->data is NULL");
        return false;
    }
    if (!dest->data) {
        report_error("dest->data is NULL");
        return false;
    }

    // deep copy memory from src to dest
    memcpy(dest->data, src->data, dest->size);

    return true;
}

bool lk_push_back(lk_array* arr, void* buf) {
    if (!arr) {
        report_error("arr cannot be NULL");
        return false;
    }

    if (!buf) {
        report_error("buf cannot be NULL");
        return false;
    }

    bool rc = lk_resize(arr, arr->size + 1);
    if (!rc) {
        report_error("lk_resize failed");
        return false;
    }

    size_t index = arr->memb_size * (arr->size - 1);
    memcpy(arr->data + index, buf, sizeof(arr->memb_size));

    return true;
}

bool lk_reserve(lk_array* arr, size_t new_size) {
    if (!arr) {
        report_error("arr cannot be NULL");
        return false;
    }

    if (new_size < arr->size) {
        report_error("cannot reserve less than is already used by arr");
        return false;
    }

    if (new_size < arr->capacity) {
        // already enough space
        // FIXME: what if we're greedy and we want to reduce capacity?
        return true;
    }

    void* new_data = LK_REALLOCARRAY(arr->data, new_size, arr->memb_size);
    if (!new_data) {
        report_error("LK_REALLOCARRAY failed");
        return false;
    }

    arr->data     = new_data;
    arr->capacity = new_size;

    return true;
}

bool lk_resize(lk_array* arr, size_t new_size) {
    if (!arr) {
        report_error("arr cannot be NULL");
        return false;
    }

    if (new_size == 0) {
        LK_FREE(arr->data);
        arr->data     = NULL;
        arr->size     = 0;
        arr->capacity = 0;
        return true;
    }

    if (new_size <= arr->capacity) {
        // already reserved, expand
        arr->size = new_size;
        return true;
    }

    void* new_data = LK_REALLOCARRAY(arr->data, new_size, arr->memb_size);
    if (!new_data) {
        report_error("LK_REALLOCARRAY failed");
        return false;
    }

    arr->data     = new_data;
    arr->size     = new_size;
    arr->capacity = new_size;

    return true;
}

void* lk_at_raw(lk_array* arr, size_t index) {
    if (!arr) {
        report_error("arr cannot be NULL");
        return NULL;
    }

    if (index >= arr->size) {
        report_error("index out of bounds");
        return NULL;
    }

    if (!arr->data) {
        report_error("arr->data is NULL ?!");
        return NULL;
    }

    return arr->data + index * arr->memb_size;
}

void* lk_get_raw(lk_array* arr) {
    if (!arr) {
        report_error("arr cannot be NULL");
        return NULL;
    }

    return arr->data;
}

bool lk_set(lk_array* arr, size_t index, void* value) {
    void* data = lk_get_raw(arr);
    if (!data) {
        report_error("lk_get_raw failed");
        return false;
    }

    if (index >= arr->size) {
        report_error("index out of range");
        return false;
    }

    data += index * arr->memb_size;
    memcpy(data, value, arr->memb_size);

    return true;
}
