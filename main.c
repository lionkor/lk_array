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
        lk_array* arr = lk_new_array(0, sizeof(int));
        test(arr != NULL);
        test(arr->data == NULL);
        test(arr->size == 0);
        test(arr->memb_size == sizeof(int));
    }

    {
        section("new array with memb_size 0");
        lk_array* arr = lk_new_array(5, 0);
        test(arr == NULL);
    }

    {
        section("new array with memb_size 1 and size 1");
        lk_array* arr = lk_new_array(1, 1);
        test(arr != NULL);
        test(arr->capacity == 1);
        test(arr->size == 1);
        test(arr->memb_size == 1);
        test(arr->data != NULL);
    }

    {
        section("deep copy array size 1 to array size 1");
        lk_array* source = lk_new_array(1, sizeof(int));
        test(source != NULL);
        lk_array* dest = lk_new_array(1, sizeof(int));
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
        lk_array* source = lk_new_array(1, sizeof(int));
        test(source != NULL);
        lk_array* dest = NULL;
        test(lk_array_deep_copy(dest, source) == false);
        test(dest == NULL);
        test(source != NULL);
    }

    {
        section("deep copy uninitialized array to uninitialized array");
        lk_array* source = NULL;
        lk_array* dest   = NULL;
        test(lk_array_deep_copy(dest, source) == false);
        test(dest == NULL);
        test(source == NULL);
    }

    {
        section("deep copy larger array to smaller array");
        lk_array* src = lk_new_array(10, sizeof(int));
        test(src != NULL);
        lk_array* dest = lk_new_array(5, sizeof(int));
        test(dest != NULL);
        test(lk_array_deep_copy(dest, src));
        test(dest->size == src->size);
        test(dest->memb_size == src->memb_size);
        test(dest->capacity == dest->size);
        test(dest->data != src->data);
    }

    {
        section("deep copy smaller array to larger array");
        lk_array* src = lk_new_array(5, sizeof(int));
        test(src != NULL);
        lk_array* dest = lk_new_array(10, sizeof(int));
        test(dest != NULL);
        test(lk_array_deep_copy(dest, src));
        test(dest->size == src->size);
        test(dest->memb_size == src->memb_size);
        test(dest->capacity == dest->size);
        test(dest->data != src->data);
    }

    {
        section("reserve larger in empty array");
        lk_array* arr = lk_new_array(0, sizeof(int));
        test(arr != NULL);
        test(lk_reserve(arr, 5));
        test(arr->size == 0);
        test(arr->memb_size == sizeof(int));
        test(arr->capacity == 5);
        test(arr->data != NULL);
    }

    {
        section("reserve too small in empty array");
        lk_array* arr = lk_new_array(10, sizeof(int));
        test(arr != NULL);
        test(lk_reserve(arr, 5) == false);
        test(arr->size == 10);
        test(arr->memb_size == sizeof(int));
        test(arr->capacity == 10);
        test(arr->data != NULL);
    }

    {
        section("reserve same size");
        lk_array* arr = lk_new_array(10, sizeof(int));
        test(arr != NULL);
        test(lk_reserve(arr, 10) == true);
        test(arr->size == 10);
        test(arr->memb_size == sizeof(int));
        test(arr->capacity == 10);
        test(arr->data != NULL);
    }

    {
        section("resize to smaller");
        lk_array* arr = lk_new_array(10, sizeof(int));
        test(arr != NULL);
        test(lk_resize(arr, 5) == true);
        test(arr->size == 5);
        test(arr->memb_size == sizeof(int));
        test(arr->capacity == 10);
        test(arr->data != NULL);
    }

    {
        section("resize to larger");
        lk_array* arr = lk_new_array(10, sizeof(int));
        test(arr != NULL);
        test(lk_resize(arr, 15) == true);
        test(arr->size == 15);
        test(arr->memb_size == sizeof(int));
        test(arr->capacity == 15);
        test(arr->data != NULL);
    }

    {
        section("resize to same size");
        lk_array* arr = lk_new_array(10, sizeof(int));
        test(arr != NULL);
        test(lk_resize(arr, 10) == true);
        test(arr->size == 10);
        test(arr->memb_size == sizeof(int));
        test(arr->capacity == 10);
        test(arr->data != NULL);
    }

    {
        section("resize NULL");
        test(lk_resize(NULL, 10) == false);
    }

    {
        section("reserve NULL");
        test(lk_reserve(NULL, 10) == false);
    }

    {
        section("push_back to empty");
        lk_array* arr = lk_new_array(0, sizeof(int));
        test(arr != NULL);
        int value = 5;
        test(lk_push_back(arr, &value));
        test(arr->size == 1);
        int* data = (int*)arr->data;
        test(data[0] == 5);
        value = 10;
        test(lk_push_back(arr, &value));
        test(arr->size == 2);
        data = (int*)arr->data;
        test(data[0] == 5);
        test(data[1] == 10);
    }

    {
        section("push_back to existing");
        lk_array* arr = lk_new_array(2, sizeof(int));
        test(arr != NULL);
        int value = 5;
        test(lk_push_back(arr, &value));
        test(arr->size == 3);
        int* data = (int*)arr->data;
        test(data[2] == 5);
        value = 10;
        test(lk_push_back(arr, &value));
        test(arr->size == 4);
        data = (int*)arr->data;
        test(data[2] == 5);
        test(data[3] == 10);
    }

    {
        section("push_back struct");
        // basically verifies that we can use any size item in the array
        struct dat {
            int   i;
            float f;
        } my_data                = { 5, 6.0f };
        struct dat my_other_data = { 100, 603.0f };

        test(my_data.i == 5);
        test(my_data.f == 6.0f);
        test(my_other_data.i == 100);
        test(my_other_data.f == 603.0f);

        lk_array* arr = lk_new_array(0, sizeof(struct dat));
        test(arr != NULL);
        test(arr->data == NULL);
        test(arr->size == 0);
        test(arr->capacity == 0);
        test(arr->memb_size == sizeof(struct dat));

        test(lk_push_back(arr, &my_data));

        test(arr != NULL);
        test(arr->data != NULL);
        test(arr->size == 1);
        test(arr->capacity == 1);
        test(arr->memb_size == sizeof(struct dat));

        struct dat* ptr = arr->data;
        test(ptr[0].i == my_data.i);
        test(ptr[0].f == my_data.f);

        test(lk_push_back(arr, &my_other_data));

        test(arr != NULL);
        test(arr->data != NULL);
        test(arr->size == 2);
        test(arr->capacity == 2);
        test(arr->memb_size == sizeof(struct dat));

        ptr = arr->data;
        test(ptr[0].i == my_data.i);
        test(ptr[0].f == my_data.f);
        test(ptr[1].i == my_other_data.i);
        test(ptr[1].f == my_other_data.f);
    }

    {
        section("push_back NULL");
        lk_array* arr = lk_new_array(0, sizeof(double));
        test(arr != NULL);
        test(lk_push_back(arr, NULL) == false);
    }
    report();
}
