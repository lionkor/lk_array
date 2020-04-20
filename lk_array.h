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

#define lk_new(type) (type*)LK_MALLOC(sizeof(type))

typedef struct {
    void*  data;
    size_t memb_size;
    size_t size;
} LkArray;

#define lk_free_array(ptr)           \
    do {                             \
        lk_free_array_internal(ptr); \
        ptr = NULL;                  \
    } while (0)

LkArray* lk_new_array(size_t size, size_t member_size);
void     lk_free_array_internal(LkArray* ptr);

#endif // LK_ARRAY_H
