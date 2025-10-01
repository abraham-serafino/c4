#include "Exception.h"

ExceptionHandler exceptionHandler = NULL;

void throwException (const cstring message) {
    if (message != NULL && exceptionHandler != NULL) {

        exceptionHandler((Exception) {
            .message = message
        });
    }
}

