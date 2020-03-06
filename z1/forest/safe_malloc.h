#include <stdlib.h>

#ifndef FOREST_SAFE_MALLOC_H
#define FOREST_SAFE_MALLOC_H

static inline void *safeMalloc(unsigned size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        exit(1); // the OS will deallocate memory
    }
    return ptr;
}

#endif // FOREST_SAFE_MALLOC_H
