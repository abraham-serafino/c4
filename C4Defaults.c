#ifndef _C4_STD_DEFAULTS_
#define _C4_STD_DEFAULTS_

#include "C4Functions.c"
#include "MemoryPool.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* std_allocate (uint size) {
    return malloc(size);
}

void* std_allocate0 (uint size) {
    return calloc(size, 1);
}

void* std_reallocate (void* original, uint size) {
    return realloc(original, size);
}

bool std_deallocate (void** data) {
    bool result = false;

    if (data != nullptr) {
        if (*data != nullptr) {
            free(*data);
            result = true;
        }

        *data = nullptr;
    }

    return result;
}

void* std_setMemory (
    void* destination,
    int value,
    uint size
) {
    return memset(destination, value, size);
}

uint std_generateUniqueNumber () {
    static uint currentHash = 1;

    return __atomic_add_fetch(
        &currentHash,
        1,
        __ATOMIC_SEQ_CST
    );
}

void std_throwException (uint errorCode) {
    char* message = "";

    switch (errorCode) {
        case ErrorCodes.InvalidPool:
            message = "Invalid pool.";
            break;

        case ErrorCodes.PageOverflow:
            message = "Page overflow.";
            break;

        case ErrorCodes.RequiredFunctionsMissing:
            message =  "You must specify setMemory() and"
                      " generateUniqueNumber() functions.";
        default:
            return;
    }

    fprintf(stderr, "\n%s\n", message);
    exit(1);
}

C4Functions getStandardDefaults () {
    throwException              = std_throwException;

    return (C4Functions) {
        .allocate               = std_allocate,
        .reallocate             = std_reallocate,
        .deallocate             = std_deallocate,
        .setMemory              = std_setMemory,
        .throwException         = std_throwException,
        .generateUniqueNumber   = std_generateUniqueNumber
    };
}

#endif // _C4_STD_DEFAULTS_
