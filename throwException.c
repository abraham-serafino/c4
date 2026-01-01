#ifndef _C4_THROW_EXCEPTION_
#define _C4_THROW_EXCEPTION_

typedef void (*ExceptionHandler)(char* message);

ExceptionHandler exceptionHandler;

void throwException (cstring message) {
    if (message != nullptr && exceptionHandler != nullptr) {
        exceptionHandler(message);
    }
}

void setExceptionHandler (ExceptionHandler newHandler) {
    exceptionHandler = newHandler;
}

#endif // _C4_THROW_EXCEPTION_
