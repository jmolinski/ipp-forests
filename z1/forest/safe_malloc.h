#include <stdlib.h>

#ifndef FOREST_SAFE_MALLOC_H
#define FOREST_SAFE_MALLOC_H

/* a wrapper for malloc
 * if malloc returns NULL then the program will exit with exit code 1
 * otherwise returns what malloc has returned */
static inline void *safeMalloc(unsigned size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        exit(1); // the OS will deallocate memory on process exit
    }
    return ptr;
}

#endif // FOREST_SAFE_MALLOC_H
