#include "throwException.h"

void throwException (cstring message) {
    if (message != NULL && exceptionHandler != NULL) {
        exceptionHandler(message);
    }
}

void setExceptionHandler (ExceptionHandler newHandler) {
    exceptionHandler = newHandler;
}
