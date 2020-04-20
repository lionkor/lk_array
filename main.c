#include <stdio.h>
#include <math.h>
#include "lk_array.h"

static void error_in_lk_array(const char* error) {
    fprintf(stderr, "lk_array error: %s\n", error);
}

#define setup()     \
    int failed = 0; \
    int passed = 0

#define report()                                                                                         \
    int total = passed + failed;                                                                         \
    printf("=====\nTotal: %d\n\tPassed: %d/%d\n\tFailed: %d/%d\n", total, passed, total, failed, total); \
    printf("Passed%%: %d%%\n", (int)round(100 * (passed / ((double)total))))

#define section(name)            \
    const char* current_section; \
    bool        ok;              \
    do {                         \
        current_section = name;  \
    } while (0)

#define test(condition)                                                              \
    do {                                                                             \
        ok = condition;                                                              \
        if (!ok) {                                                                   \
            ++failed;                                                                \
            printf("case failed in \"%s\" : ( %s )\n", current_section, #condition); \
        } else {                                                                     \
            ++passed;                                                                \
        }                                                                            \
    } while (0)

int main() {
    setup();

    lk_setup_error_callback(error_in_lk_array);

    {
        section("testing tests");
        test(true);
    }

    {
        section("new array with size 0");
        LkArray* arr = lk_new_array(0, sizeof(int));
        test(arr != NULL);
        test(arr->data == NULL);
        test(arr->size == 0);
        test(arr->memb_size == sizeof(int));
    }

    {
        section("new array with memb_size 0");
        LkArray* arr = lk_new_array(5, 0);
        test(arr == NULL);
    }

    {
        section("new array with memb_size 1 and size 1");
        LkArray* arr = lk_new_array(1, 1);
        test(arr != NULL);
        test(arr->capacity == 1);
        test(arr->size == 1);
        test(arr->memb_size == 1);
        test(arr->data != NULL);
    }

    {
        section("deep copy array size 1 to array size 1");
        LkArray* source = lk_new_array(1, sizeof(int));
        test(source != NULL);
        LkArray* dest = lk_new_array(1, sizeof(int));
        test(dest != NULL);
        test(lk_array_deep_copy(dest, source));
        test(dest->size == 1);
        test(dest->memb_size == sizeof(int));
        test(dest->capacity == 1);
        // not the same data pointer (would be a shallow copy)
        test(dest->data != source->data);
        // make sure we dont segfault on access
        int* a = (int*)dest->data;
        a[0]   = 1;
        int* b = (int*)source->data;
        b[0]   = 2;
        test(a[0] != b[0]);
    }

    {
        section("deep copy array size 1 to uninitialized array");
        LkArray* source = lk_new_array(1, sizeof(int));
        test(source != NULL);
        LkArray* dest = NULL;
        test(lk_array_deep_copy(dest, source) == false);
        test(dest == NULL);
        test(source != NULL);
    }

    {
        section("deep copy uninitialized array to uninitialized array");
        LkArray* source = NULL;
        LkArray* dest   = NULL;
        test(lk_array_deep_copy(dest, source) == false);
        test(dest == NULL);
        test(source == NULL);
    }

    {
        section("deep copy larger array to smaller array");
        LkArray* src = lk_new_array(10, sizeof(int));
        test(src != NULL);
        LkArray* dest = lk_new_array(5, sizeof(int));
        test(dest != NULL);
        test(lk_array_deep_copy(dest, src));
        test(dest->size == src->size);
        test(dest->memb_size == src->memb_size);
        test(dest->capacity == dest->size);
        test(dest->data != src->data);
    }

    {
        section("deep copy smaller array to larger array");
        LkArray* src = lk_new_array(5, sizeof(int));
        test(src != NULL);
        LkArray* dest = lk_new_array(10, sizeof(int));
        test(dest != NULL);
        test(lk_array_deep_copy(dest, src));
        test(dest->size == src->size);
        test(dest->memb_size == src->memb_size);
        test(dest->capacity == dest->size);
        test(dest->data != src->data);
    }

    {
        section("reserve larger in empty array");
        LkArray* arr = lk_new_array(0, sizeof(int));
        test(arr != NULL);
        test(lk_reserve(arr, 5));
        test(arr->size == 0);
        test(arr->memb_size == sizeof(int));
        test(arr->capacity == 5);
        test(arr->data != NULL);
    }
    report();
}
