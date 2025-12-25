#include "c4.h"
#include "c4defaults.h"

void increment(Int x) {
    (deref (x))++;
}

int main () {
    initializeStandardDefaults();

    var myInt           = box (Int, 100);
    var bottlesOfBeer   = ref (Int, myInt);

    // bottlesOfBeer = 67;            // should not compile
    // *bottlesOfBeer = 9;            // should not compile

    unbox (myInt)--;
    debug("\n%d\n", deref (bottlesOfBeer)); // '99'

    var myOtherInt  = heap  (Int, 99);
    var secondRef   =       myOtherInt;

    increment(myOtherInt);
    debug("%d\n\n", deref (secondRef)); // '100'

    destroy (myOtherInt);

    // comment out the condition to test "Invalid reference."
    unless (isNull(secondRef)) {
        debug(
            "%d\n",
            deref (secondRef) // throws "Invalid reference."
        );
    }

    // destroy() quietly refuses to free the same memory twice:
    destroy (secondRef);

    // Unfortunately, the compiler won't let us do this:
    // if (secondRef == null) { /* ... */ }

    return 0;
}
