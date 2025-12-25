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
#define null ((void*) 0)
#define UINT32_MAX 0xFFFFFFFF

// typedef struct DataType {
//     const cstring name;
//     uint size;
// } DataType;

// #define defineType(T) \
//     const DataType* T##Type = & (DataType) {\
//         .name = #T, \
//         .size = sizeof(T) \
//     };

// #define declareType(T) \
//     extern const DataType* T##Type;

#define object(name) \
    typedef struct name name;\
    declareType(name);\
    struct name

#define option(name) \
    typedef enum name name;\
    enum name

// declareType(int8);
// declareType(uint8);
// declareType(byte);
// declareType(boolean);
// declareType(int16);
// declareType(uint16);
// declareType(int32);
// declareType(natural);
// declareType(double);
// declareType(number);
// declareType(int64);
// declareType(uint64);
// declareType(superlong);
// declareType(superulong);
// declareType(int);
// declareType(char);

#endif // _C4_TYPES_H_
