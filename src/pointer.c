#include "pointer.h"

const pointer null = {};

uint calculateBytes (uint count, uint itemSize) {
    var totalBytes = (uint64) count * itemSize;

    if (totalBytes > UINT32_MAX) {
        throwException("Number of bytes exceeds the 32-bit limit.");
        return -1;
    }

    return (uint) totalBytes;
}

pointer createArray (
    const TypeDescriptor* itemType,
    uint length,
    boolean shouldInitialize
) {

    if (itemType == NULL) {
        throwException("Invalid type.");
        return null;
    }

    var totalBytes = calculateBytes(length, (*itemType).size);

    if (totalBytes <= 0) {
        return null;
    }

    var data = allocate(
        sizeof(uint64) + totalBytes,
        shouldInitialize
    );

    if (data == NULL) {
        throwException("Unable to reserve memory.");        
        return null;
    }

    var this = (pointer) {
        .data = data,
        .type = itemType,
        .hash = generateRandomNumber()
    };

    // Storing a unique identifier provides a way for other
    // functions to verify that this.data still points to the
    // original, 'unfreed' data.
    *((uint64*) this.data) = this.hash;

    return this;
}

boolean isNullOrInvalid (pointer this) {
    if (this.data == NULL) {
        return true;
    }

    if (*((uint64*) this.data) != this.hash) {
        // The data this object points to is no longer valid.
        // Maybe it has been freed or overwritten, or maybe
        // this is a "dangling pointer."
        return true;
    }

    return false;
}

boolean destroyPointer (pointer* this) {
    if (this == NULL) {
        return false;
    }

    if (isNullOrInvalid(*this)) {
        return false;
    }

    // Zero out the hash header to indicate that
    // the pointer is no longer "valid."
    var totalSize = sizeof(uint64);

    forEach (i, from(0), upTo(totalSize)) {
        *((byte*) (*this).data + i) = 0;
    }

    deallocate(&((*this).data));

    return true;
}

void* unboxPointer (pointer this, const TypeDescriptor* type) {
    if (isNullOrInvalid(this)) {
        throwException("Invalid reference.");
        return NULL;
    }

    if (type == NULL) {
        throwException("Specified type is invalid.");
        return NULL;
    }

    if (this.type == NULL) {
        throwException("Reference type is invalid.");
        return NULL;
    }

    if (this.type != type) {
        throwException("Reference does not match expected type.");
        return NULL;
    }

    return (void*) ((byte*) this.data + sizeof(uint64));
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

void setRandomGenerator (RandomGenerator generator) {
    generateRandomNumber = generator;
}
