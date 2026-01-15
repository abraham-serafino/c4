#ifndef _C4_COLLECTION_
#define _C4_COLLECTION_

object (StoreItem) {
    UniqueUint id;
    StoreItem* nextFreeItem;
};

object (MemoryBuffer) {
    ArraySize capacity;
    ArraySize size;
    UniqueUint currentId;
    ArrayIndex currentIndex;
    double growthFactor;
    ErrorCode errorCode;
    MemCopier copyMem;
    UniqueUint (*generateUniqueHash) (MemoryBuffer buffer)
};

object (StoreOptions) {
    ArraySize capacity;

    void* (*memCopier) (
        void* destination,
        void* source,
        ArraySize size
    );

    double growthFactor;

    UniqueUint (*uniqueHashGenerator) (MemoryBuffer buffer);
};

#define defineStore(T, name) \
    typedef struct name##StoreItem name##StoreItem; \
    typedef struct name##Store name##Store; \
    struct name##StoreItem { \
        StoreItem; \
        T value; \
    } \
    struct name##Store { \
        MemoryBuffer; \
        name##StoreItem* firstFreeItem; \
        name##StoreItem items[]; \
    }

UniqueUint generateUniqueHash (MemoryBuffer buffer) {
    return __atomic_add_fetch(
        &collection.currentHash,
        1,
        __ATOMIC_SEQ_CST
    );
}

#define createCollection(T, ...) ({ \
    var _args = (BufferOptions) { \
        __VA_ARGS__ \
    }; \
    \
    if (_args.allocator == nullptr) { \
        throwException("Invalid allocator."); \
    } else if (_args.memCopier == nullptr) { \
        throwException("Invalid mem copier.");
    } else if (_args.growthFactor > 1) { \
        throwException("Growth factor must be < 1."); \
    } else { \
        MemoryBuffer* buffer = (MemoryBuffer*) \
            _args.allocator( \
                sizeof(T##Store) + ( \ 
                    _args.capacity * sizeof(T##StoreItem) \
                ) \
            ); \
        \
        if (buffer == nullptr) { \
            throwException("Invalid memory."); \
        } else { \
            *buffer = (MemoryBuffer) { \
                .capacity = _args.capacity, \
                .size = 0, \
                .currentHash = 1, \
                .currentIndex = 0, \
                .growthFactor = _args.growthFactor, \
                .copyMem = _args.memCopier, \
            \
                .generateUniqueHash = _args.uniqueHashGenerator ? \
                    _args.uniqueHashGenerator : \
                    generateUniqueHash \
            }; \
            \
            buffer; \
        } \
    } \
})

object (BufferHandle) {
    UniqueUint id;
    ArrayIndex index;
};

#define itemAt(buffer, handle) ({ \
    if (buffer == nullptr) {
        throwException("Invalid buffer.");
    } \
    if (handle.index > buffer.size) {
        throwException("Array index out of bounds.\n");
    } \
    if ((*buffer).items[index].id != handle.id) { \
        throwException("Invalid buffer handle."); \
    } \
    ((*buffer).items[index].value); \
})

#endif // _C4_COLLECTION_
