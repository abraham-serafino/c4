#include "MemoryPool.h"

MemoryPool* createMemoryPool (MemoryPoolOptions options) {
    var blockSize    = options.blockSize;
    var blockCount   = options.blockCount;
    var wasAllocated = false;

    if (blockSize == 0 || blockCount == 0) {
        return null;
    }

    var memorySource = (MemoryPool*) options.memorySource;

    if (memorySource == null) {
        wasAllocated = true;

        memorySource = (MemoryPool*) allocate(
            TOTAL_POOL_SIZE(blockSize, blockCount)
        );
    }

    if (memorySource == null) {
        throwException("Insufficient memory.");
        return null;
    }

    *memorySource           = (MemoryPool) {
        .blockSize          = blockSize,
        .blockCount         = blockCount,
        .growOnCommand      = options.growOnCommand,
        .wasAllocated       = wasAllocated,
        .freeBlockIndex     = 1,

        .currentBlock       = (MemoryBlock*) &(
            (*memorySource).freeBlocks[0]
        )
    };

    return memorySource;
}

boolean clearMemoryPool (MemoryPool* pool) {
    if (pool == null) {
        return false;
    }

    until ((*pool).currentBlock == null) {
        var block               = (*pool).currentBlock;
        (*pool).currentBlock    = (*block).next;
        (*block).next           = (*pool).firstFreeBlock;
        (*pool).firstFreeBlock  = block;
    }

    boolean result = nextFreeBlock(pool) != null;

    if (result) {
        (*pool).generation++;
    }

    return result;
}

boolean destroyMemoryPool (MemoryPool** _pool) {
    if (_pool == null || *_pool == null) {
        return false;
    }

    var pool = **_pool;
    var success = true;

    if (pool.wasAllocated) {
        return deallocate((void**) _pool);
    }

    while (success && pool.currentBlock != null) {
        var block = pool.currentBlock;
        pool.currentBlock = (*block).next;

        if ((*block).wasAllocated) {
            success = deallocate((void**) &block);
        }
    }

    while (success && pool.currentBlock != null) {
        var block = pool.firstFreeBlock;
        pool.firstFreeBlock = (*block).next;

        if ((*block).wasAllocated) {
            success = deallocate((void**) &block);
        }
    }

    return success;
}

void* reservePoolMemory (MemoryPool* pool, uint size) {
    if (pool == null) {
      return allocate0(size);
    }

    var blockSize = (*pool).blockSize;

    if (size > blockSize) {
        throwException("Maximum allocation size exceeded.");
        return null;
    }

    var currentBlock    = (*pool).currentBlock;
    var offset          = ALIGN((*currentBlock).offset);

    if (currentBlock == null || blockSize - offset < size) {
        currentBlock = nextFreeBlock(pool);
        offset = 0;
    }

    if (currentBlock == null) {
        return null;
    }

    var ptr = (void*)  &((*currentBlock).data[offset]);
    setMemory (ptr, 0, size);
    (*currentBlock).offset += size;

    return ptr;
}

MemoryBlock* nextFreeBlock (MemoryPool* pool) {
    if (pool == null) {
        return null;
    }

    var nextBlock       = (MemoryBlock*) null;
    var freeBlockIndex  = (*pool).freeBlockIndex;

    var totalSize = ALIGN(
        TOTAL_BLOCK_SIZE((*pool).blockSize)
    );

    if (freeBlockIndex < (*pool).blockCount) {
        nextBlock = (MemoryBlock*) (
            (byte*) (*pool).freeBlocks +
            (freeBlockIndex * totalSize)
        );

        (*pool).freeBlockIndex++;

    } else if ((*pool).firstFreeBlock != null) {
        nextBlock               = (*pool).firstFreeBlock;
        (*pool).firstFreeBlock  = (*nextBlock).next;

    } else if ((*pool).growOnCommand) {
        nextBlock = allocate(totalSize);
    }

    if (nextBlock == null) {
        return null;
    }

    *nextBlock      = (MemoryBlock) {
        .offset     = 0,
        .next       = (*pool).currentBlock
    };

    (*pool).currentBlock = nextBlock;
    return  nextBlock;
}
