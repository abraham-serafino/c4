#include <stdio.h>
#include "Store.h"

extern ExceptionHandler exceptionHandler;

int main () {

    var store = newStore();

    int cleanup (int returnValue) {
        deleteStore(&store);
        exceptionHandler = NULL;
        return returnValue;
    }

    void handleException (Exception exception) {
        fprintf(stderr, exception.message);
        fprintf(stderr, "\n");

        exit(cleanup(1));
    }

    exceptionHandler = &handleException;

    for (var i = 5; i > 0; --i) {
        newStoreItem(&store, int, {i});
    }

    for (var j = 0; j < store.size; ++j) {
        var item = getStoreItem(&store, j);

        if (! isNull(item)) {
            printf("%d\n", $(int, item));
        }
    }

    return cleanup(0);
}
