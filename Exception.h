#ifndef _EXCEPTION_H
#define _EXCEPTION_H

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

#endif // _EXCEPTION_H
