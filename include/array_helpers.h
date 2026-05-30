#ifndef ARRAY_HELPERS_H
#define ARRAY_HELPERS_H

#include <string.h>

#include "vector.h"

int vcontains(vector_str* arr, const char* string) {
    for (size_t i = 0; i <= arr->size; ++i) {
        if (strcmp(vector_item(*arr, i), string) == 0) return 0;
    }

    return 1;
}

char* acontains(const char* arr[], const char* string, size_t size) {
    for (size_t i = 0; i <= size; ++i) {
        const char* curr = arr[i];
        if (strcmp(curr, string) == 0) return curr;
    }

    return NULL;
}

#endif  // !ARRAY_HELPERS_H
