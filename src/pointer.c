#include "pointer.h"

uint64 generateUniqueHash() {
    static uint64 currentHash = 1;

    return __atomic_add_fetch(
        &currentHash,
        1,
        __ATOMIC_SEQ_CST
    );
}

void setAllocator (Allocator allocator) {
    allocate = allocator;
}

void setReallocator (Reallocator reallocator) {
    reallocate = reallocator;
}

void setDeallocator (Deallocator deallocator) {
    deallocate = deallocator;
}
