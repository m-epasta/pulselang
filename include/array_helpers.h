#ifndef ARRAY_HELPERS_H
#define ARRAY_HELPERS_H

#include "vector.h"

int contains(vector_str* arr, char* string) {
    for (size_t i = 0; i <= arr->size; ++i) {
        if (strcmp(vector_item(*arr, i), string) == 0) return 0;
    }

    return 1;
}

#endif  // !ARRAY_HELPERS_H
