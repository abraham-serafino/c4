#include "c4defaults.h"

ExceptionHandler exceptionHandler;

Allocator       allocate;
Allocator       allocate0;
Reallocator     reallocate;
Deallocator     deallocate;
MemorySetter    setMemory;

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
    return malloc(size);
}

void* stdZeroAllocator (uint size) {
    return calloc(size, 1);
}

void* stdReallocator (void* original, uint size) {
    return realloc(original, size);
}

boolean stdDeallocator (void** data) {
    boolean result = false;

    if (data != null) {
        if (*data != null) {
            free(*data);
            result = true;
        }

        *data = null;
    }

    return result;
}

void* stdMemorySetter (
    void* destination,
    int value,
    uint size
) {
    return memset(destination, value, size);
}

void initializeStandardDefaults () {
    exceptionHandler = defaultExceptionHandler;

    allocate    = stdAllocator;
    allocate0   = stdZeroAllocator;
    reallocate  = stdReallocator;
    deallocate  = stdDeallocator;
    setMemory   = stdMemorySetter;
}

// void initializeGcDefaults () {
//     GC_INIT();

//     exceptionHandler = defaultExceptionHandler;

//     allocate = gcAllocator;
//     reallocate = gcReallocator;
//     deallocate = gcDeallocator;
// }
