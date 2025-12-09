#ifndef _C4_POINTER_H_
#define _C4_POINTER_H_

#include "gc.h"
#include "c4.h"

object (pointer) {
    const TypeDescriptor* type;
    void* data;
    uint64 hash;
    uint length;
};

extern const pointer null;

uint calculateBytes(uint count, uint itemSize);

pointer createArray (
    const TypeDescriptor* itemType,
    uint length,
    boolean shouldInitialize
);

#define create(T) createArray(T##Type, 1, true)
#define create_uninitialized(T) createArray(T##Type, 1, false)

boolean destroyPointer (pointer* this);
#define destroy(this) destroyPointer(&this)
void* unboxPointer (pointer this, const TypeDescriptor* type);
#define unbox(value, T) *((T*) unboxPointer(value, T##Type))
boolean isNullOrInvalid (pointer this);
#define isNull(this) isNullOrInvalid(this)

typedef void* (*Allocator)(uint size, boolean shouldInitialize);
extern Allocator allocate;
void setAllocator (Allocator allocator);

typedef void* (*Reallocator)(void* original, uint size);
extern Reallocator reallocate;
void setReallocator (Reallocator reallocator);

typedef void (*Deallocator)(void** data);
extern Deallocator deallocate;
void setDeallocator (Deallocator deallocator);

typedef uint64 (*RandomGenerator)();
extern RandomGenerator generateRandomNumber;
void setRandomGenerator (RandomGenerator generator);

#endif // _C4_POINTER_H_
