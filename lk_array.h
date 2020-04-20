#ifndef LK_ARRAY_H
#define LK_ARRAY_H

#include "userdef_memory.h"
/* From this point onwards only macros should be used for
 * memory allocation / deallocation / reallocation.
 * Namely these are
 * - LK_MALLOC,        
 * - LK_FREE,
 * - LK_CALLOC,
 * - LK_REALLOC,
 * - LK_REALLOCARRAY.
 * These may be overridden by defining them before this include.
 */

#include <stdbool.h>

#define lk_new(type) (type*)LK_MALLOC(sizeof(type))

typedef void (*callback_ptr)(const char*);

typedef struct {
    void*  data;
    size_t memb_size;
    size_t size;
    size_t capacity;
} LkArray;

#define lk_free_array(ptr)           \
    do {                             \
        lk_free_array_internal(ptr); \
        ptr = NULL;                  \
    } while (0)

/// Allocates a new array using LK_MALLOC and LK_CALLOC.
/// The returned pointer may be NULL on error (look at error handling below).
/// The returned pointer, if not NULL, has to be free'd using lk_free_array.
LkArray* lk_new_array(size_t size, size_t member_size);

/// Internal free() function for LkArrays. Use lk_free_array instead.
void lk_free_array_internal(LkArray* ptr);

bool lk_array_deep_copy(LkArray* dest, LkArray* src);

bool lk_push_back(LkArray* arr, void* buf);

bool lk_reserve(LkArray* arr, size_t new_size);
bool lk_resize(LkArray* arr, size_t new_size);

/*
 * Error handling: 
 *
 * To define your own error handler, simply pass a function pointer
 * to lk_set_error_callback. The signature
 *      void(const char*)
 * is expected.
 *
 * To set a default error callback (fprintf to stderr) call 
 * lk_set_error_callback_stderr. This is great for setting up a quick project.
 */
/// Sets the error callback to be called when an error occurs.
/// Signature: void(const char*)
void lk_setup_error_callback(callback_ptr fn);
/// Resets the error callback to one which prints the error to stderr.
void lk_setup_error_callback_stderr(void);

#endif // LK_ARRAY_H
