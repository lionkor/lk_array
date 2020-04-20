#include <stdio.h>
#include "lk_array.h"

int main(int argc, char** argv) {
    LkArray* arr = lk_new_array(5, sizeof(int));
    lk_free_array(arr);
}
