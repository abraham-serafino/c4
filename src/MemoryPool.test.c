#include "MemoryPool.h"

#define BLOCK_SIZE  1024
#define BLOCK_COUNT 8

#define TOTAL_POOL_SIZE ( \
    POOL_HEADER_SIZE +  ( \
        BLOCK_COUNT  * \
        ALIGN(BLOCK_HEADER_SIZE + BLOCK_SIZE) \
    ) \
)

byte    globalBuffer        [TOTAL_POOL_SIZE]   = {};

int main() {
    var pool        = createMemoryPool((MemoryPoolOptions) {
        .blockSize    = BLOCK_SIZE,
        .blockCount   = BLOCK_COUNT,
        .memorySource = globalBuffer
    });

    printf("Hello, World!\n");
    return 0;
}
