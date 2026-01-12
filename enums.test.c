#include "types.c"
#include <stdio.h>

#define ScoreMap(X) \
    X(A, 100)       \
    X(B, 90)        \
    X(C, 80)        \
    X(D, 70)        \
    X(F, 60)

defineStaticMap(ScoreMap, Scores);

int testEnums() {
    printf("%d %d %d\n", Scores.A, Scores.C, Scores.F);
    return 0;
}
