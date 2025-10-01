#include <stdio.h>
#include "ArrayList.h"

extern ExceptionHandler exceptionHandler;

int main () {
    var store = newArrayList(0);

    int cleanup (int returnValue) {
        deleteList(&store);
        exceptionHandler = NULL;
        return returnValue;
    }

    void handleException (Exception exception) {
        fprintf(stderr, exception.message);
        fprintf(stderr, "\n");

        exit(cleanup(1));
    }

    exceptionHandler = &handleException;

    for (var i = 0; i < 5; ++i) {
        var item = pushItem(&store, new(int, {i + 1}));
    }

    var storeLength = store.length;

    for (var j = 0; j < storeLength; ++j) {
        var item = shiftItemOut(&store);

        if (! isNull(item)) {
            printf("removeItem(%d): %d\n", j, unbox(int, item));
        } else {
            printf("removeItem(%d): null.\n", j);
        }
    }

    return cleanup(0);
}

