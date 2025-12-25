#include "c4.h"
#include "c4defaults.h"

object (Person_t) {
    cstring name;
    uint age;
};

defineBoxedType(Person_t, Person);
definePointerType(Person);

Person newPerson(cstring name, uint age) {
    var result = heap (Person, {});

    (deref (result)) = (Person_t) {
        .name   = name,
        .age    = age
    };

    return result;
}

void incrementAge(Person person) {
    (deref (person)).age++;
}

int main () {
    initializeStandardDefaults();

    var myInt           = box (Int, 100);
    var bottlesOfBeer   = ref (Int, myInt);

    // bottlesOfBeer = 67; // should not compile
    // *bottlesOfBeer = 9; // should not compile

    unbox (myInt)--;
    debug("\n%d\n", deref (bottlesOfBeer)); // '99'

    var bob         = newPerson("Bob", 20);
    var secondRef   = bob;

    incrementAge(bob);
    debug("%d\n\n", (deref (secondRef)).age); // '21'

    destroy (bob);

    // comment out the condition to test "Invalid reference."
    unless (isNull(secondRef)) {
        debug(
            "Hello, %s!\n",

            // throws "Invalid reference."
            (deref (secondRef)).name
        );
    }

    // destroy quietly refuses to free the same memory twice
    destroy (secondRef);

    // Unfortunately, the compiler won't let us do this:
    // if (secondRef == null) { /* ... */ }

    return 0;
}
