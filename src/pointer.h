#ifndef _C4_POINTER_H_
#define _C4_POINTER_H_

#include "c4.h"

typedef void*   (*Allocator)    (uint size);
typedef void*   (*Reallocator)  (void* original, uint size);
typedef void    (*Deallocator)  (void** data);

uint64 generateUniqueHash   ();
void   setAllocator         (Allocator allocator);
void   setReallocator       (Reallocator reallocator);
void   setDeallocator       (Deallocator deallocator);

extern Allocator    allocate;
extern Reallocator  reallocate;
extern Deallocator  deallocate;

#define defineBoxedType(T, name) \
    typedef struct { \
        uint64 hash; \
        T data; \
    } name##Box

#define definePointerType(T) \
    typedef struct { \
        uint64 hash; \
        T##Box* data; \
    } T

#define box(T, value) ( \
    (T##Box) { \
         .hash = generateUniqueHash(), \
         .data = value \
    } \
)

#define unbox(box) box.data

#define ref(T, value) ( \
    (T) { \
        .hash = value.hash, \
        .data = &value \
    } \
)

#define heap(T, value) ({ \
    T##Box* _temp = (T##Box*) allocate(sizeof(T##Box)); \
    if (!_temp) throwException("Out of memory"); \
    *_temp = box (T, value); \
    (T) { \
        .hash = _temp->hash, \
        .data = _temp \
    }; \
})

#define destroy(ptr) { \
    if (! isNull(ptr)) { \
        (ptr).data->hash = 0; \
        deallocate((void**) &(ptr).data); \
    } \
}

#define deref(ptr) *({ \
    const typeof(ptr) _p = (ptr); \
    if (isNull(_p)) { \
        throwException("Invalid reference."); \
    } \
    &(_p.data->data); \
})

#define isNull(ptr) ( \
    (ptr).data == null || (ptr).data->hash != (ptr).hash \
)

defineBoxedType     (int,        Int);
defineBoxedType     (int8,       Int8);
defineBoxedType     (uint8,      Uint8);
defineBoxedType     (byte,       Byte);
defineBoxedType     (boolean,    Boolean);
defineBoxedType     (int16,      Int16);
defineBoxedType     (uint16,     Uint16);
defineBoxedType     (int32,      Int32);
defineBoxedType     (natural,    Natural);
defineBoxedType     (double,     Double);
defineBoxedType     (number,     Number);
defineBoxedType     (int64,      Int64);
defineBoxedType     (uint64,     Uint64);
defineBoxedType     (superlong,  Superlong);
defineBoxedType     (superulong, Superulong);
defineBoxedType     (char,       Char);

definePointerType   (Int);
definePointerType   (Int8);
definePointerType   (Uint8);
definePointerType   (Byte);
definePointerType   (Boolean);
definePointerType   (Int16);
definePointerType   (Uint16);
definePointerType   (Int32);
definePointerType   (Natural);
definePointerType   (Double);
definePointerType   (Number);
definePointerType   (Int64);
definePointerType   (Uint64);
definePointerType   (Superlong);
definePointerType   (Superulong);
definePointerType   (Char);

#endif // _C4_POINTER_H_
