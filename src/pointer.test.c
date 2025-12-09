#include "c4.h"
#include "c4defaults.h"

int main () {
    initializeStandardDefaults();

    var bottlesOfBeer = create (int);
    var secondRef = bottlesOfBeer;
    var thirdRef = secondRef;

    // bottlesOfBeer = 67; // does not compile
    // *bottlesOfBeer = 9; // does not compile

    // uncomment to test "Reference does not match expected type."
    // unbox(bottlesOfBeer, char) = 'c';

    // works just like *bottlesOfBeer = 99
    unbox (bottlesOfBeer, int) = 99;

    debug("\n%d\n", unbox (secondRef, int)); // '99'
    destroy (bottlesOfBeer);

    // destroy() quietly refuses to free the same memory twice:
    destroy (secondRef);

    // comment out the condition to test "Invalid reference."
    unless (isNull (thirdRef)) {
        debug("(first invalid reference):\n");

        debug(
            "%d\n",
            unbox (thirdRef, int) // throws "Invalid reference."
        );
    }

    // Unfortunately, the compiler won't let us do this:
    // if (thirdRef == null) { /* ... */ }

    // turn on the garbage collector
    initializeGcDefaults();

    // 'pointer' is type agnostic, so thirdRef can be reused
    // as a reference to a different type:
    thirdRef = create_uninitialized (char);
            // ^ slightly faster than create()

    var fourthRef = thirdRef;

    // Careful - destroy() will invalidate *both* references!
    // destroy (thirdRef);

    // When GC is turned on, you should usually do this, instead:
    thirdRef = null;

    // Now we can trust GC not to free the memory yet,
    // because fourthRef still points to it.

    debug(
        "after setting thirdRef back to null: '%c'\n",

        // dereference, assign, and evaluate in a single step.
        unbox (fourthRef, char) = 'A'
    );

    // This will break the last link to the allocated memory,
    // and GC will deallocate it on the next pass.
    fourthRef = null;

    // uncomment to test "Invalid reference."
    // unbox (fourthRef, char);

    debug(
        "after setting fourthRef to null: %s\n\n",

        !isNull (fourthRef) ?
            // '&' casts the unboxed value to a "string" (char*)
            &(unbox (fourthRef, char)) :

            "null"
    );

    return 0;
}
