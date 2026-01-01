// Dependencies:
//
// throwException.c
// types.c
// c4Utils.c
//

#ifndef _C4_MEMORY_POOL_
#define _C4_MEMORY_POOL_

#define ALIGN(value) (\
     ((value)           + (alignof(void*))  - 1) & \
    ~((alignof(void*))                      - 1) \
)

#define TOTAL_BLOCK_SIZE(blockSize) \
    sizeof(MemoryBlock) + (blockSize)

#define TOTAL_POOL_SIZE(blockSize, blockCount) (\
    sizeof(MemoryPool) + \
    TOTAL_BLOCK_SIZE(blockSize) * blockCount \
)

object (MemoryBlock) {
  uint          offset;
  boolean       wasAllocated;
  MemoryBlock*  next;
  byte          data[];
};

object (MemoryPool) {
  uint          blockSize;
  uint          blockCount;
  boolean       growOnCommand;
  boolean       wasAllocated;
  uint          generation;
  uint          freeBlockIndex;
  MemoryBlock*  currentBlock;
  MemoryBlock*  firstFreeBlock;
  byte          freeBlocks[];
};

object (MemoryPoolOptions) {
  uint          blockSize;
  uint          blockCount;
  boolean       growOnCommand;
  byte*         memorySource;
};

MemoryPool* createMemoryPool (MemoryPoolOptions options) {
    var blockSize    = options.blockSize;
    var blockCount   = options.blockCount;
    var wasAllocated = false;

    if (blockSize == 0 || blockCount == 0) {
        return nullptr;
    }

    var memorySource = (MemoryPool*) options.memorySource;

    if (memorySource == nullptr) {
        wasAllocated = true;

        memorySource = (MemoryPool*) allocate(
            TOTAL_POOL_SIZE(blockSize, blockCount)
        );
    }

    if (memorySource == nullptr) {
        throwException("Insufficient memory.");
        return nullptr;
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

MemoryBlock* nextFreeBlock (MemoryPool* pool) {
    if (pool == nullptr) {
        return nullptr;
    }

    var nextBlock       = (MemoryBlock*) nullptr;
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

    } else if ((*pool).firstFreeBlock != nullptr) {
        nextBlock               = (*pool).firstFreeBlock;
        (*pool).firstFreeBlock  = (*nextBlock).next;

    } else if ((*pool).growOnCommand) {
        nextBlock = allocate(totalSize);
    }

    if (nextBlock == nullptr) {
        return nullptr;
    }

    *nextBlock      = (MemoryBlock) {
        .offset     = 0,
        .next       = (*pool).currentBlock
    };

    (*pool).currentBlock = nextBlock;
    return  nextBlock;
}

boolean clearMemoryPool (MemoryPool* pool) {
    if (pool == nullptr) {
        return false;
    }

    until ((*pool).currentBlock == nullptr) {
        var block               = (*pool).currentBlock;
        (*pool).currentBlock    = (*block).next;
        (*block).next           = (*pool).firstFreeBlock;
        (*pool).firstFreeBlock  = block;
    }

    boolean result = nextFreeBlock(pool) != nullptr;

    if (result) {
        (*pool).generation++;
    }

    return result;
}

boolean destroyMemoryPool (MemoryPool** _pool) {
    if (_pool == nullptr || *_pool == nullptr) {
        return false;
    }

    var pool = **_pool;
    var success = true;

    if (pool.wasAllocated) {
        return deallocate((void**) _pool);
    }

    while (success && pool.currentBlock != nullptr) {
        var block = pool.currentBlock;
        pool.currentBlock = (*block).next;

        if ((*block).wasAllocated) {
            success = deallocate((void**) &block);
        }
    }

    while (success && pool.currentBlock != nullptr) {
        var block = pool.firstFreeBlock;
        pool.firstFreeBlock = (*block).next;

        if ((*block).wasAllocated) {
            success = deallocate((void**) &block);
        }
    }

    return success;
}

void* reservePoolMemory (MemoryPool* pool, uint size) {
    if (pool == nullptr) {
      return allocate0(size);
    }

    var blockSize = (*pool).blockSize;

    if (size > blockSize) {
        throwException("Maximum allocation size exceeded.");
        return nullptr;
    }

    var currentBlock    = (*pool).currentBlock;
    var offset          = ALIGN((*currentBlock).offset);

    if (currentBlock == null || blockSize - offset < size) {
        currentBlock = nextFreeBlock(pool);
        offset = 0;
    }

    if (currentBlock == nullptr) {
        return nullptr;
    }

    var ptr = (void*)  &((*currentBlock).data[offset]);
    setMemory (ptr, 0, size);
    (*currentBlock).offset += size;

    return ptr;
}

#endif // _C4_MEMORY_POOL_
