#ifndef _C4_POINTER_
#define _C4_POINTER_

#include "types.c"

#define defineBoxedType(T, name) \
    typedef struct { \
        uint64 hash; \
        T data; \
    } name##Box; \
    typedef struct { \
        uint64 hash; \
        name##Box* data; \
    } name

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

#define deref(ptr) *({ \
    const typeof(ptr) _p = (ptr); \
    if (isNull(_p)) { \
        throwException("Invalid reference."); \
    } \
    &(_p.data->data); \
})

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

#define isNull(ptr) ( \
    (ptr).data == nullptr || (ptr).data->hash != (ptr).hash \
)

// defineBoxedType     (int,        Int);
// defineBoxedType     (int8,       Int8);
// defineBoxedType     (uint8,      Uint8);
// defineBoxedType     (byte,       Byte);
// defineBoxedType     (boolean,    Boolean);
// defineBoxedType     (int16,      Int16);
// defineBoxedType     (uint16,     Uint16);
// defineBoxedType     (int32,      Int32);
// defineBoxedType     (natural,    Natural);
// defineBoxedType     (double,     Double);
// defineBoxedType     (number,     Number);
// defineBoxedType     (int64,      Int64);
// defineBoxedType     (uint64,     Uint64);
// defineBoxedType     (superlong,  Superlong);
// defineBoxedType     (superulong, Superulong);
// defineBoxedType     (char,       Char);

#endif // _C4_POINTER_
