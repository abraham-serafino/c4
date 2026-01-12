#ifndef _C4_TYPES_
#define _C4_TYPES_

typedef            _BitInt(  8) int8;
typedef            _BitInt( 16) int16;
typedef            _BitInt( 24) int24;
typedef            _BitInt( 32) int32;
typedef            _BitInt( 64) int64;
typedef            _BitInt(128) int128;

typedef unsigned   _BitInt(  8) uint8;
typedef unsigned   _BitInt( 16) uint16;
typedef unsigned   _BitInt( 24) uint24;
typedef unsigned   _BitInt( 32) uint32;
typedef unsigned   _BitInt( 64) uint64;
typedef unsigned   _BitInt(128) uint128;

typedef             uint8       byte;
typedef             double      number;

typedef char*                   cstring;
typedef typeof(sizeof(0))       uint;
typedef uint                    Natural;

#define var                     auto

#define object(name)            \
    typedef struct name name;   \
    struct name

#define _declareNameValueStruct(name, value) uint name;
#define _defineNameValueStruct(name, value) .name = value,

#define defineStaticMap(valueMap, name) constexpr   \
    struct { valueMap(_declareNameValueStruct)   }  \
    name = { valueMap(_defineNameValueStruct)    }

#endif // _C4_TYPES_
