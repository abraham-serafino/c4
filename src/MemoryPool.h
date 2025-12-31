#ifndef _C4_MEMORY_POOL_H_
#define _C4_MEMORY_POOL_H_

#include "c4.h"

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

MemoryPool*     createMemoryPool  (MemoryPoolOptions options);
boolean         clearMemoryPool   (MemoryPool* pool);
boolean         destroyMemoryPool (MemoryPool** pool);
void*           reservePoolMemory (MemoryPool* pool, uint size);
MemoryBlock*    nextFreeBlock     (MemoryPool* pool);
boolean         clearCurrentBlock (MemoryPool* pool);

#endif // _C4_MEMORY_POOL_H_
