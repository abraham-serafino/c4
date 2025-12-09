#ifndef _C4_THROW_EXCEPTION_H_
#define _C4_THROW_EXCEPTION_H_

#include "c4.h"

typedef void (*ExceptionHandler)(cstring message);
extern ExceptionHandler exceptionHandler;
void setExceptionHandler (ExceptionHandler newHandler);

void throwException (cstring message);

#endif // _C4_THROW_EXCEPTION_H_
