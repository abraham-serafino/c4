#include "c4defaults.h"

ExceptionHandler exceptionHandler;
RandomGenerator generateRandomNumber;

Allocator allocate;
Reallocator reallocate;
Deallocator deallocate;

void defaultExceptionHandler (cstring message) {
    fprintf(stderr, "\n%s\n", message);
    exit(1);
}

uint64 defaultRandomGenerator () {
    return time(NULL);
}

void* gcAllocator (uint size, boolean shouldInitialize) {
    return GC_MALLOC(size);
}

void* gcReallocator (void* original, uint size) {
    return GC_REALLOC(original, size);
}

void gcDeallocator (void** data) {
    if (data != NULL && *data != NULL) {
        *data = NULL;
    }
}

void* stdAllocator (uint size, boolean shouldInitialize) {
    if (shouldInitialize) {
        return calloc(size, 1);
    } else {
        return malloc(size);
    }
}

void* stdReallocator (void* original, uint size) {
    return realloc(original, size);
}

void stdDeallocator (void** data) {
    if (data != NULL) {
        if (*data != NULL) {
            free(*data);
        }

        *data = NULL;
    }
}

void initializeStandardDefaults () {
    exceptionHandler = defaultExceptionHandler;
    generateRandomNumber = defaultRandomGenerator;

    allocate = stdAllocator;
    reallocate = stdReallocator;
    deallocate = stdDeallocator;
}

void initializeGcDefaults () {
    GC_INIT();

    exceptionHandler = defaultExceptionHandler;
    generateRandomNumber = defaultRandomGenerator;

    allocate = gcAllocator;
    reallocate = gcReallocator;
    deallocate = gcDeallocator;
}
