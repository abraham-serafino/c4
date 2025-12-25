#include "c4defaults.h"

ExceptionHandler exceptionHandler;

Allocator   allocate;
Reallocator reallocate;
Deallocator deallocate;

void defaultExceptionHandler (cstring message) {
    fprintf(stderr, "\n%s\n", message);
    exit(1);
}

// void* gcAllocator (uint size, boolean shouldInitialize) {
//     return GC_MALLOC(size);
// }

// void* gcReallocator (void* original, uint size) {
//     return GC_REALLOC(original, size);
// }

// void gcDeallocator (void** data) {
//     if (data != NULL && *data != NULL) {
//         *data = NULL;
//     }
// }

void* stdAllocator (uint size) {
    return calloc(size, 1);
}

void* stdReallocator (void* original, uint size) {
    return realloc(original, size);
}

void stdDeallocator (void** data) {
    if (data != null) {
        if (*data != null) {
            free(*data);
        }

        *data = null;
    }
}

void initializeStandardDefaults () {
    exceptionHandler = defaultExceptionHandler;

    allocate    = stdAllocator;
    reallocate  = stdReallocator;
    deallocate  = stdDeallocator;
}

// void initializeGcDefaults () {
//     GC_INIT();

//     exceptionHandler = defaultExceptionHandler;

//     allocate = gcAllocator;
//     reallocate = gcReallocator;
//     deallocate = gcDeallocator;
// }
