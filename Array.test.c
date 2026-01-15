#include "c4.h"
#include "c4defaults.h"
#include "Array.h"

int main() {
    initializeStandardDefaults();

    var numbers = newArrayOf(int, 1, 2, 3);
    debug("%d\n", unboxArray(numbers, 2, int));

    var otherNumbers = createArraySlice(numbers, 1, 2);
    destroyArray(numbers);

    debug(
        "numbers was %s\n",
        isNull(numbers) ? "destroyed" : "not destroyed"
    );

    setArrayItem(otherNumbers, 0, int, 1);
    setArrayItem(otherNumbers, 1, int, 5);

    forEach (i, from(0),
                upTo(2) // include the sentinel             
    ) {
        debug("%d ", unboxArray(otherNumbers, i, int));
    }

    debug("\n");

    resizeArray(otherNumbers, 5);
    shiftArrayRight(otherNumbers.innerArray, 1, intType, 3);
    setArrayItems(otherNumbers, 1, int, 2, 3, 4);

    forEach (i, from(0),
                upTo(6)
    ) {
        debug("%d ", unboxArray(otherNumbers, i, int));
    }

    debug("\n");

    resizeArray(otherNumbers, 7);
    shiftArrayRight(otherNumbers.innerArray, 2, intType, 2);

    forEach (i, from(0),
                upTo(8)
    ) {
        debug("%d ", unboxArray(otherNumbers, i, int));
    }

    debug("\n");

    return 0;
}
