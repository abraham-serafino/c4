#ifndef _C4_TYPES_
#define _C4_TYPES_

typedef signed char         int8;
typedef unsigned char       uint8;
typedef uint8               byte;
typedef byte                boolean;
typedef short               int16;
typedef unsigned short      uint16;
typedef uint16              word;

#ifdef _C4_16_BIT_
    typedef long            int32;
    typedef unsigned long   uint32;
#else // int is 32 bits
    typedef int             int32;
    typedef unsigned int    uint32;
#endif

typedef uint32              uint;
typedef long long           int64;
typedef unsigned long long  uint64;

typedef uint32              natural;
typedef double              number;

typedef __int128            superlong; // requires GCC
typedef unsigned __int128   superulong; // requires GCC

typedef char*               cstring;

#define var                 auto
#define bitfield(i)         _BitInt(i)
#define UINT32_MAX          0xFFFFFFFF

#define object(name) \
    typedef struct name name; \
    struct name

#define option(name) \
    typedef enum name name; \
    enum name

#endif // _C4_TYPES_
