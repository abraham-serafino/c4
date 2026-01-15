#ifndef _C4_ARRAY_H_
#define _C4_ARRAY_H_

#include "c4.h"

object (InnerArray) {
    void* data;
    const DataType* type;
    uint capacity;
    uint length;
};

object (Array) {
    InnerArray* innerArray;
    const DataType* type;
    uint64 hash;
};

extern const Array null;

Array allocateArray (const DataType* type, uint length);
#define arrayOf(T, length) allocateArray(T##Type, length)

void* unboxArrayIndex (
    Array this,
    uint index,
    const DataType* type
);

#define unboxArray(this, index, T) \
    *((T*) unboxArrayIndex(this, index, T##Type))

boolean setMultipleArrayItems(
    Array this,
    uint index,
    const DataType* type,
    uint itemCount,
    void* source
);

#define setArrayItems(this, index, T, ...) \
    setMultipleArrayItems( \
        this,\
        index,\
        T##Type,\
        sizeof((T[]) { __VA_ARGS__ }) / sizeof(T),\
        (T[]) { __VA_ARGS__ } \
    )

#define setArrayItem(this, index, T, source) \
    setArrayItems(this, index, T, source)

Array createArraySlice(
    Array this,
    uint index,
    uint itemCount
);

#define copyArray(this) createArraySlice(this, 0, this.length)
#define copyArrayItem(this, index) createArraySlice(this, index, 1)

Array createArrayFromItems(
    const DataType* type,
    uint length,
    void* items
);

#define newArrayOf(T, ...) \
    createArrayFromItems( \
        (T##Type),\
        (sizeof((T[]) {__VA_ARGS__}) / sizeof(T)),\
        (T[]) {__VA_ARGS__ } \
    )

boolean resizeArray (Array this, uint newMinimumSize);

boolean shiftArrayRight (
    InnerArray* array,
    uint index,
    const DataType* type,
    uint distance
);

boolean destroyArray (Array this);

typedef uint64 (*UniqueNumberGenerator)();
extern UniqueNumberGenerator generateUniqueNumber;
void setUniqueNumberGenerator (UniqueNumberGenerator generator);

InnerArray* getInnerArray (Array array, const DataType* type);
boolean isNullOrInvalid (Array this);
#define isNull(this) isNullOrInvalid(this)

uint calculateBytes (uint count, uint itemSize);

void copyMemTo (
    void* destination,
    void* source,
    uint32 totalBytes,
    boolean shouldCopyInReverse
);

#endif // _C4_ARRAY_H_
