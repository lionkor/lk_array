# `lk_array` - A simple dynamic C array

`lk_array` is a simple, dynamically self resizing array class for C.

It's written with simplicity and safety in mind, and comes with error reporting builtin.
Allows for bound-checking safe access, resizing, reserving, iteration, custom allocators, *any* size data, and so much more!
Error handling is made easy with a builtin error reporting callback and sane error behaviour (no assert's, segfaults, etc).

All it takes is adding `lk_array.c` and `lk_array.h` to your project!

**Comes with tests!**

## Use cases

`lk_array` is suitable specifically for holding non-primitive data types. The generic nature and lack of
specializations (like premade int arrays) is tailored to this. 

## Examples

Array holding integers. Example allocates, writes, does a push_back, reads, frees:
```cpp
lk_array* my_ints = lk_new_array(10, sizeof(int));

for (int i = 0; i < 10; ++i) {
    int value = i * 2;
    // sets my_ints at index i to value
    lk_set(my_ints, i, &value);
}

int value = 600;

// grows array by 1 and inserts the value at the end
lk_push_back(my_ints, &value);

for (int i = 0; i < 10 + 1; ++i) {
    // retrieves the value at index i and casts it to int
    int* ptr = lk_at(arr, int, i);
    printf("%d\n", *ptr);
}

lk_free_array(my_ints);
```

This example is rather silly as holding a bunch of ints is not the intended use case, but it demonstrates the possibilities.

