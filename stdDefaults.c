// Dependencies
//
// stdlib.h
// stdio.h
// string.h
//

#ifndef _C4_STD_DEFAULTS_
#define _C4_STD_DEFAULTS_

void defaultExceptionHandler (cstring message) {
    fprintf(stderr, "\n%s\n", message);
    exit(1);
}

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

    if (data != nullptr) {
        if (*data != nullptr) {
            free(*data);
            result = true;
        }

        *data = nullptr;
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
    exceptionHandler    = defaultExceptionHandler;
    allocate            = stdAllocator;
    allocate0           = stdZeroAllocator;
    reallocate          = stdReallocator;
    deallocate          = stdDeallocator;
    setMemory           = stdMemorySetter;
}

#endif // _C4_STD_DEFAULTS_
