/*
 * userdef_memory.h
 *
 * The following macros may be defined before this.
 * The idea is that you can use a custom memory manager instead
 * of the "default" malloc/free. The signatures in comments are 
 * the expected signatures.
 *
 * Note that reallocarray is non-POSIX and may not be supported by 
 * all custom memory managers. If this is the case for you, just
 * define LK_REALLOCARRAY_NONE. This will enable a custom reallocarray
 * that uses the other allocators.
 */

#ifndef LK_MALLOC
#include <stdlib.h>
// void* LK_MALLOC(size_t size)
#define LK_MALLOC malloc
#endif // LK_MALLOC

#ifndef LK_FREE
#include <stdlib.h>
// void free(void* ptr)
#define LK_FREE free
#endif // LK_FREE

#ifndef LK_REALLOC
#include <stdlib.h>
// void* realloc(void* ptr, size_t size)
#define LK_REALLOC realloc
#endif // LK_REALLOC

#ifndef LK_CALLOC
#include <stdlib.h>
// void* calloc(size_t nmemb, size_t size)
#define LK_CALLOC calloc
#endif // LK_CALLOC

#ifndef LK_REALLOCARRAY
// define this to say "my memory manager does not support this".
#ifdef LK_REALLOCARRAY_NONE
#error "TODO: Implement custom reallocarray"
#else
#include <stdlib.h>
// void* reallocarray(void* ptr, size_t nmemb, size_t size)
#define LK_REALLOCARRAY reallocarray
#endif // LK_REALLOCARRAY_NONE
#endif // LK_REALLOCARRAY

