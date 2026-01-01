// unity builds ftw
#include "types.c"
#include "throwException.c"
#include <stdio.h>
#include "c4utils.c"
#include "pointer.c"
#include "c4defaults.c"
#include "MemoryPool.c"
#include <stdlib.h>
#include <string.h>
#include "stdDefaults.c"

#define BLOCK_SIZE   16
#define BLOCK_COUNT  5

byte globalBuffer[
    TOTAL_POOL_SIZE(BLOCK_SIZE, BLOCK_COUNT)
] = {};

int main() {
    initializeStandardDefaults();

    var pool            = createMemoryPool((MemoryPoolOptions) {
        .blockSize      = BLOCK_SIZE,
        .blockCount     = BLOCK_COUNT,
        .memorySource   = globalBuffer,

        // technically unnecessary - including for documentation
        .growOnCommand  = false 
    });

    MemoryBlock* currentBlock = (*pool).currentBlock;

    debug (
        "(*currentBlock).offset == 0: %d\n",
        (*currentBlock).offset == 0
    );

    debug ("currentBlock: %p\n", currentBlock);

    var bob = (char*) reservePoolMemory(pool, sizeof("Bob"));
    debug ("bob != null: %d\n", bob != nullptr);
    strncpy(bob, "Bob", sizeof("Bob"));

    var alice = (char*) reservePoolMemory(pool, sizeof("Alice"));
    debug ("alice != null: %d\n", alice != nullptr);
    strncpy(alice, "Alice", sizeof("Alice"));

    debug (
        "(*currentBlock).offset == 10: %d (%d)\n",
        (*currentBlock).offset == 10,
        (*currentBlock).offset
    );

    debug (
        "(*pool).currentBlock == currentBlock: %d\n",
        (*pool).currentBlock == currentBlock
    );

    var randomNumber = (int*) reservePoolMemory(pool, sizeof(int));

    debug (
        "(*pool).currentBlock != currentBlock: %d\n",
        (*pool).currentBlock != currentBlock
    );

    currentBlock = (*pool).currentBlock;

    debug (
        "(*currentBlock).offset == 4: %d\n",
        (*currentBlock).offset == 4
    );

    clearMemoryPool(pool);
    currentBlock = (*pool).currentBlock;

    debug (
        "(*currentBlock).offset == 0: %d (%d)\n",
        (*currentBlock).offset == 0,
        (*currentBlock).offset
    );

    // uncomment to test "Maximum allocation size exceded."
    // reservePoolMemory(
    //     pool,
    //     sizeof("abcdef123456789aaaaa99999")
    // );

    var result = (char*) nullptr;

    forEach (i, from(0), upTo(6)) {
        result = (char*) reservePoolMemory(
            pool,
            sizeof("abcdef123456789")
        );

        if (result == nullptr) {
            break;
        }
    }

    debug (
        "result == null: %d (%p)\n",
        result == nullptr,
        result
    );

    (*pool).growOnCommand = true;

    result = (char*) reservePoolMemory(
        pool,
        sizeof("abcdef123456789")
    );

    debug (
        "result == null: %d (%p)\n",
        result == nullptr,
        result
    );

    destroyMemoryPool(&pool);

    return 0;
}
