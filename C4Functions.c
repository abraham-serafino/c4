#ifndef _C4_DEFAULTS_
#define _C4_DEFAULTS_

#include "types.c"

typedef void*   (*Allocator)        (uint size);
typedef void*   (*Reallocator)      (void* original, uint size);
typedef bool    (*Deallocator)      (void** data);
typedef void    (*ExceptionHandler) (uint errorCode);

typedef void*   (*MemorySetter)     (
    void*       destination,
    int         value,
    uint        size
);

typedef uint (*UniqueNumberGenerator) ();

object (C4Functions) {
    Allocator               allocate;
    Allocator               allocate0;
    Reallocator             reallocate;
    Deallocator             deallocate;
    MemorySetter            setMemory;
    ExceptionHandler        throwException;
    UniqueNumberGenerator   generateUniqueNumber;
};

extern ExceptionHandler throwException;

#endif // _C4_DEFAULTS_
