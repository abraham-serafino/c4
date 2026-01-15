#include "Array.h"

const Array null = {};

Array allocateArray (const DataType* type, uint length) {
    if (type == NULL) {
        throwException("Invalid type.");
        return null;
    }

    var innerArray = allocate(sizeof(InnerArray), true);

    if (innerArray == NULL) {
        throwException("Unable to reserve memory.");        
        return null;
    }

    var totalBytes = calculateBytes(length + 1, (*type).size);

    if (totalBytes <= 0) {
        return null;
    }

    var data = allocate(
        sizeof(uint64) + totalBytes,
        true
    );

    if (data == NULL) {
        throwException("Unable to reserve memory.");        
        return null;
    }

    var hash = generateUniqueNumber();
    *((uint64*) data) = hash;

    *((InnerArray*) innerArray) = (InnerArray) {
        .type = type,
        .capacity = length,
        .length = length,
        .data = data
    };

    var this = (Array) {
        .type = type,
        .innerArray = innerArray,
        .hash = hash
    };

    return this;
}

void* unboxArrayIndex (
    Array array,
    uint index,
    const DataType* type
) {
    var this = getInnerArray(array, type);

    if (this == NULL) {
        return NULL;
    }

    if (index > (*this).length) {
        throwException("Array index out of bounds.");
        return NULL;
    }

    var byteOffset = calculateBytes(index, (*type).size);

    if (byteOffset < 0) {
        return NULL;
    }

    return (void*) (
        (byte*) (*this).data + sizeof(uint64) + byteOffset
    );
}

boolean _setMultipleArrayItems(
    InnerArray* this,
    uint index,
    const DataType* type,
    uint itemCount,
    void* source,
    boolean shouldCopyInReverse
) {
    var base = (byte*) (*this).data + sizeof(uint64);
    var itemSize = (*type).size;
    var offset = calculateBytes(index, itemSize);

    if (offset < 0) {
        return false;
    }

    var totalBytes = calculateBytes(itemCount, itemSize);

    if (totalBytes < 0) {
        return false;
    }

    var destination = base + offset;

    copyMemTo(
        destination,
        from(source),
        totalBytes,
        shouldCopyInReverse
    );

    return true;
}

boolean setMultipleArrayItems(
    Array array,
    uint index,
    const DataType* type,
    uint itemCount,
    void* source
) {
    var this = getInnerArray(array, type);

    if (this == NULL) {
        return false;
    }

    if (items == NULL) {
        throwException("Invalid item(s).")
        return false;
    }

    if (index + itemCount > (*this).length) {
        throwException("Array index out of bounds.");
        return false;
    }

    return _setMultipleArrayItems(
        this,
        index,
        type,
        itemCount,
        source,
        false
    );
}

Array createArraySlice(
    Array array,
    uint index,
    uint itemCount
) {
    var this = getInnerArray(array, array.type);

    if (this == NULL) {
        return null;
    }

    if (index + itemCount > (*this).capacity) {
        throwException("Array capacity overflow.");
        return null;
    }

    var itemType = (*this).type;
    var result = allocateArray(itemType, itemCount);
    var offset = calculateBytes(index, (*itemType).size);

    if (offset < 0) {
        return null;
    }

    _setMultipleArrayItems(
        result.innerArray,
        0,
        itemType,
        itemCount,
        (byte*) (*this).data + sizeof(uint64) + offset,
        false
    );

    return result;
}

Array createArrayFromItems(
    const DataType* type,
    uint length,
    void* items
) {
    if (items == NULL) {
        throwException("Invalid item(s).");
        return null;
    }

    Array this = allocateArray(type, length);

    if (isNullOrInvalid(this)) {
        return null;
    }

    _setMultipleArrayItems(
        this.innerArray,
        0,
        type,
        length,
        items,
        false
    );

    return this;
}

boolean _resizeArray (
    InnerArray* this,
    uint itemSize,
    uint newCapacity
) {
    var totalBytes = calculateBytes(newCapacity + 1, itemSize);

    if (totalBytes < 0) {
        return false;
    }

    if (totalBytes == 0) {
        return true; // nothing to do
    }

    var newData = reallocate(
        (*this).data,
        totalBytes + sizeof(uint64)
    );

    if (newData == NULL) {
        throwException("Unable to resize array.");        
        return false;
    }

    (*this).data = newData;
    (*this).capacity = newCapacity;

    return true;
}

boolean resizeArray (Array array, uint newSize) {
    var this = getInnerArray(array, array.type);

    if (this == NULL) {
        return false;
    }

    return _resizeArray(this, (*((*this).type)).size, newSize);
}

boolean shiftArrayRight (
    InnerArray* this,
    uint index,
    const DataType* type,
    uint distance
) {
    var length = (*this).length;

    if (index >= length) {
        throwException("Array index out of bounds.");
        return false;
    }

    var itemSize = (*type).size;
    var sourceOffset = calculateBytes(index, (*type).size);

    if (sourceOffset < 0) {
        return false;
    }

    var destinationIndex = index + distance;

    _setMultipleArrayItems(
        this,
        destinationIndex,
        type,
        length - index,
        (byte*) (*this).data + sizeof(uint64) + sourceOffset,
        true
    );

    (*this).length += distance;
    return true;
}

boolean insertArrayItems(
    Array array,
    uint index,
    const DataType* type,
    uint itemCount,
    void* items
) {
    var this = getInnerArray(array, type);

    if (this == NULL) {
        return false;
    }

    if (items == NULL) {
        throwException("Invalid item(s).");
        return false;
    }

    var newCapacity = (*this).capacity;
    var length = (*this).length;
    var itemSize = (*type).size;

    while (newCapacity <= length + itemCount) {
        newCapacity *= 2;
    }

    unless (_resizeArray(this, itemSize, newCapacity)) {
        return false;
    }

    unless (shiftArrayRight(this, index, type, itemCount)) {
        return false;
    }

    unless (_setMultipleArrayItems(
        this,
        index,
        type,
        itemCount,
        items,
        false
    )) {
        return false;
    }

    return true;
}

boolean destroyArray (Array array) {
    if (isNullOrInvalid(array)) {
        return false;
    }

    var this = array.innerArray;
    var totalSize = sizeof(uint64);

    // Zero out the hash header to indicate that
    // the Array is no longer "valid."
    *((uint64*) (*this).data) = 0;

    deallocate(&((*this).data));
    return true;
}

void setUniqueNumberGenerator (UniqueNumberGenerator generator) {
    generateUniqueNumber = generator;
}

InnerArray* getInnerArray (Array array, const DataType* type) {
    if (isNullOrInvalid(array)) {
        throwException("Invalid reference.");
        return NULL;
    }

    if (type == NULL) {
        throwException("Specified type is invalid.");
        return NULL;
    }

    var this = array.innerArray;

    if ((*this).type == NULL) {
        throwException("Reference type is invalid.");
        return NULL;
    }

    if ((*this).type != type) {
        throwException("Reference does not match expected type.");
        return NULL;
    }

    if ((*this).length > (*this).capacity) {
        throwException("Array data has been corrupted.");
        return NULL;
    }

    return this;
}

// boolean shiftArrayLeft (
//     pointer array,
//     uint index,
//     uint itemCount
// ) {
//     var this = getArrayFromPointer(array);

//     if (this == NULL) {
//         return false;
//     }

//     var targetIndex = index + 1 - itemCount;

//     if (targetIndex <= 0) {
//         targetIndex = 0;
//     }

//     var arrayType = (*this).type;
//     var itemSize = (*arrayType).size;

//     var copyTo = calculateBytes(targetIndex, itemSize);

//     if (copyTo < 0) {
//         return false;
//     }

//     var startAt = calculateBytes(index, itemSize);

//     if (startAt < 0) {
//         return false;
//     }

//     var destination = (byte*) (*this).items + copyTo;
//     var source = (byte*) (*this).items + startAt;
//     var totalBytes = calculateBytes((*this).length - index, itemSize);

//     if (totalBytes < 0) {
//         return false;
//     }

//     if (totalBytes == 0) {
//         return true; // nothing to do
//     }

//     copyMemTo (destination, from(source), totalBytes);

//     return true;
// }

boolean isNullOrInvalid (Array array) {
    var this = array.innerArray;

    if (this == NULL || (*this).data == NULL) {
        return true;
    }

    if (*((uint64*) (*this).data) != array.hash) {
        // The data this object points to is no longer valid.
        // Maybe it has been freed or overwritten, or maybe
        // this is a "dangling pointer."
        return true;
    }

    return false;
}

uint calculateBytes (uint count, uint itemSize) {
    var totalBytes = (uint64) count * itemSize;

    if (totalBytes > UINT32_MAX) {
        throwException("Number of bytes exceeds the 32-bit limit.");
        return -1;
    }

    return (uint) totalBytes;
}

void copyMemTo (
    void* destination,
    void* source,
    uint32 totalBytes,
    boolean shouldCopyInReverse,
) {
    if (source == NULL || destination == NULL) {
        throwException("Invalid reference.");
    }

    if (shouldCopyInReverse) {
        forEach (i, from(totalBytes - 1), 
                    downTo(0)
        ) {
            *((byte*) destination + i) = *((byte*) source + i);
        }
    } else {
        forEach (i, from(0), 
                    upTo(totalBytes)
        ) {
            *((byte*) destination + i) = *((byte*) source + i);
        }
    }
}
