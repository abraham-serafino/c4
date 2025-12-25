#include "throwException.h"

void throwException (cstring message) {
    if (message != null && exceptionHandler != null) {
        exceptionHandler(message);
    }
}

void setExceptionHandler (ExceptionHandler newHandler) {
    exceptionHandler = newHandler;
}
