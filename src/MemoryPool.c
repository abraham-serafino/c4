#include "MemoryPool.h"

MemoryPool* createMemoryPool (MemoryPoolOptions options) {
  var blockSize    = options.blockSize;
  var blockCount   = options.blockCount;

  if (blockSize == 0 || blockCount == 0) {
    return null;
  }

  var memorySource    = (MemoryPool*) options.memorySource;

  if (memorySource == null) {
      memorySource = (MemoryPool*) allocate(
          sizeof(MemoryPool) + (
            (blockSize + sizeof(MemoryBlock)) * blockCount
          )
      );
  }

  if (memorySource == null) {
    throwException("Insufficient memory.");
    return null;
  }

  *memorySource     = (MemoryPool) {
    .blockSize      = blockSize,
    .blockCount     = blockCount,
    .generation     = 0,
    .growOnCommand  = options.growOnCommand,
    .freeBlockIndex = 1,
    .currentBlock   = (MemoryBlock*) &((*memorySource).freeBlocks[0])
  };

  if (blockCount > 1) {
    (*memorySource).firstFreeBlock = (MemoryBlock*) &(
        (*memorySource).freeBlocks[0]
    );
  }

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

boolean destroyMemoryPool (MemoryPool** pool) {
    return deallocate((void**) pool);
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
        sizeof(MemoryBlock) + (*pool).blockSize
    );

    if (freeBlockIndex < (*pool).blockCount) {
        nextBlock = (MemoryBlock*) (
            (byte*) (*pool).freeBlocks +
            (freeBlockIndex * totalSize)
        );

        (*pool).freeBlockIndex++;

    } else if ((*pool).firstFreeBlock != null) {
        nextBlock = (*pool).firstFreeBlock;
        (*pool).firstFreeBlock = (*nextBlock).next;
        (*nextBlock).next = null;

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

    (*pool).currentBlock =  nextBlock;

    return nextBlock;
}
