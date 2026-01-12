#include <stdio.h>
#include "loops.c"
#include "types.c"

int testLoops() {
    count_up (from(1), to(10), as(i)) {
        printf("%lu\n", i);
    }

    count_down (from(10), to(1), as(j)) {
        printf("%lu\n", j);
    }

    var val = 0;

    until (val == 5) {
        ++val;
    }

    var scores = (int[]) { 
        92,
        85,
        100
    };

    for_each (item_from (scores), as (int score)) {
        printf("Score: %d\n", score);
    }

    return 0;
}
