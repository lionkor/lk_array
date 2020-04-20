#ifndef LK_ARRAY_H
#define LK_ARRAY_H

/*
 * lk_array.h
 *
 * Defines interface for handling the lk_array structure.
 * 
 * Read the README for usage examples.
 */

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

/// Macro to simplify malloc's
#define lk_new(type) (type*)LK_MALLOC(sizeof(type))

typedef void (*callback_ptr)(const char*);

/// Structure that holds all data concerning an array in this library.
typedef struct {
    void*  data;
    size_t memb_size;
    size_t size;
    size_t capacity;
} lk_array;

/// Macro to use for freeing lk_arrays. Ensures pointer gets sanitized in
/// an attempt to ensure use-after-free.
#define lk_free_array(ptr)           \
    do {                             \
        lk_free_array_internal(ptr); \
        ptr = NULL;                  \
    } while (0)

/// Allocates a new array using LK_MALLOC and LK_CALLOC.
/// The returned pointer may be NULL on error (look at error handling below).
/// The returned pointer, if not NULL, has to be free'd using lk_free_array.
lk_array* lk_new_array(size_t size, size_t member_size);

/// Internal free() function for lk_arrays. Use lk_free_array instead.
void lk_free_array_internal(lk_array* ptr);

/// Copies data from src to dest.
/// Will fail if dest or src are NULL.
bool lk_array_deep_copy(lk_array* dest, lk_array* src);

/// Pushes back (appends) the element pointed to by buf.
/// buf may *not* be NULL. Only arr->memb_size bytes will be copied
/// from buf. arr will be resized in the process.
/// Returns false on error, true on success.
bool lk_push_back(lk_array* arr, void* buf);

/// Reserves (pre-allocates) enough memory to hold new_size elements of size
/// arr->memb_size. Allows for resizing up to new_size without
/// new allocations.
/// Will fail if new_size < arr->size.
/// Only increases capacity.
bool lk_reserve(lk_array* arr, size_t new_size);

/// Resizes the array to hold new_size elements of size arr->memb_size.
/// Discards elements if new_size < arr->size (shrinking).
/// Note that this isn't memory-greedy and will *not* reallocate if shrinking
/// occurs. The capacity may remain the same after this.
bool lk_resize(lk_array* arr, size_t new_size);

/// Macro for simple access to values of specific type.
/// Usage: int* my_value = lk_at(the_array, int, 5)
/// to get index 5 as int*. Beware: returns NULL on failure.
#define lk_at(arr, type, index) \
    (type*)lk_at_raw(arr, index)

/// Returns a void pointer to the specified index in the array.
/// Returns NULL on failure (does bounds checking).
void* lk_at_raw(lk_array* arr, size_t index);

/// Equivalent of arr->data, except returns NULL if arr is NULL.
void* lk_get_raw(lk_array* arr);

/// Macro to simplify getting a type* (array of type `type`).
/// Returns NULL on failure. sizeof(type) should match arr->memb_size.
#define lk_get(arr, type) \
    (type*)lk_get_raw(arr)

/// Sets the value at the given index in the array.
/// Does bounds checking, returns false on failure.
bool lk_set(lk_array* arr, size_t index, void* value);

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
