#ifndef _C4_MEMORY_POOL_H_
#define _C4_MEMORY_POOL_H_

#include "c4.h"

#define ALIGN(value) (\
     ((value)           + (alignof(void*))  - 1) & \
    ~((alignof(void*))                      - 1) \
)

#define POOL_HEADER_SIZE sizeof(MemoryPool)
#define BLOCK_HEADER_SIZE sizeof(MemoryBlock)

object (MemoryBlock) {
  uint         offset;
  MemoryBlock*  next;
  byte          data[];
};

object (MemoryPool) {
  uint          blockSize;
  uint          blockCount;
  boolean       growOnCommand;
  uint          generation;
  uint          freeBlockIndex;
  MemoryBlock*  currentBlock;
  MemoryBlock*  firstFreeBlock;
  byte          freeBlocks[];
};

object (MemoryPoolOptions) {
  uint      blockSize;
  uint      blockCount;
  boolean   growOnCommand;
  byte*     memorySource;
};

MemoryPool*   createMemoryPool  (MemoryPoolOptions options);
boolean       clearMemoryPool   (MemoryPool* pool);
boolean       destroyMemoryPool (MemoryPool** pool);
void*         reservePoolMemory (MemoryPool* pool, uint size);
MemoryBlock*  nextFreeBlock     (MemoryPool* pool);

#endif // _C4_MEMORY_POOL_H_
