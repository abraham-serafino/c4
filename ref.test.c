#include <stdio.h>
#include "ref.h"

extern ExceptionHandler exceptionHandler;

object (Person) {
    int age;
};

int main() {
    Int price = new (int, {99});
    Int price2 = price;
    boxed (Person) alice;

    int cleanup (int returnValue) {
        delete (price);
        delete (alice);
        return returnValue;
    }

    void handleException (Exception exception) {
        fprintf(stderr, exception.message);
        fprintf(stderr, "\n");

        exit(cleanup(1));
    }

    exceptionHandler = &handleException;

    $(int, price)++;
    printf("%d\n", $(int, price2));

    delete(price2);
    printf("%s\n", isNull(price) ? "true" : "false");

    alice = new(Person, { .age = 20 });
    $(Person, alice).age++;

    printf("%d\n", $(Person, alice).age);

    return cleanup(0);
}
