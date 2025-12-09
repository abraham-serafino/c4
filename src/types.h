#ifndef _C4_TYPES_H_
#define _C4_TYPES_H_

typedef signed char int8;
typedef unsigned char uint8;
typedef uint8 byte;
typedef byte boolean;
typedef short int16;
typedef unsigned short uint16;
typedef uint16 word;

#ifdef _C4_16_BIT_
    typedef long int32;
    typedef unsigned long uint32;
#else // int is 32 bits
    typedef int int32;
    typedef unsigned int uint32;
#endif

typedef uint32 uint;

typedef long long int64;
typedef unsigned long long uint64;

typedef uint32 natural;
typedef double number;

typedef __int128 superlong; // requires GCC
typedef unsigned __int128 superulong; // requires GCC

typedef char* cstring;

#define var auto
#define bitfield(i) _BitInt(i)
#define NULL ((void*) 0)
#define UINT32_MAX 0xFFFFFFFF
#define UINT64_MAX 0xFFFFFFFFFFFFFFFFULL

typedef struct TypeDescriptor TypeDescriptor;

struct TypeDescriptor {
    const cstring typeName;
    uint size;
};

#define defineType(T) \
    const TypeDescriptor* T##Type = & (TypeDescriptor) {\
        .typeName = #T, \
        .size = sizeof(T) \
    };

#define defineTypeHeader(T) \
    extern const TypeDescriptor* T##Type;

#define object(name) \
    typedef struct name name;\
    struct name

#define option(name) \
    typedef enum name name;\
    enum name

defineTypeHeader(int8);
defineTypeHeader(uint8);
defineTypeHeader(byte);
defineTypeHeader(boolean);
defineTypeHeader(int16);
defineTypeHeader(uint16);
defineTypeHeader(int32);
defineTypeHeader(natural);
defineTypeHeader(double);
defineTypeHeader(number);
defineTypeHeader(int64);
defineTypeHeader(uint64);
defineTypeHeader(superlong);
defineTypeHeader(superulong);
defineTypeHeader(int);
defineTypeHeader(char);

#endif // _C4_TYPES_H_
