#include <stdio.h>
#include "Store.h"

extern ExceptionHandler exceptionHandler;

int main () {
    var store = newStore();
    var value = (natural*) NULL;

    int cleanup (int returnValue) {
        deleteStore(&store);

        return returnValue;
    }

    void handleException (Exception exception) {
        fprintf(stderr, exception.message);
        fprintf(stderr, "\n");

        exit(cleanup(1));
    }

    exceptionHandler = &handleException;

    for (var i = 5; i > 0; --i) {
        newStoreItem(natural, &store, &i);
    }

    for (var j = 0; j < store.size; ++j) {
        var item = getStoreItem(natural, &store, j);

        if (! (*item).isNull) {
            value = (natural*) unbox(natural, item);
            printf("%d\n", (*value));
        }
    }

    return cleanup(0);
}
