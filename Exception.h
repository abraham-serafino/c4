#ifndef _C4_EXCEPTION_H
#define _C4_EXCEPTION_H

#include "types.h"

option (ExceptionType) {
    Success,
    NullPointerException,
    MemoryException,
    ValidationException,
    ArrayIndexException
};

object (Exception) {
    const cstring message;
    ExceptionType type;
};

typedef void (*ExceptionHandler)(Exception);

void throwException (const cstring message);

#endif // _C4_EXCEPTION_H
