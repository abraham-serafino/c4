#ifndef _C4_TYPES_H
#define _C4_TYPES_H

#include <stdint.h>
#include <stddef.h>

#define int8 int8_t
#define byte uint8_t
#define boolean _Bool
#define int16 int16_t
#define word uint16_t
#define int int32_t
#define uint uint32_t
#define natural uint32_t
#define number double
#define ulong unsigned __int128 // may not exist on all systems
#define superlong __int128 // may not exist on all systems
#define cstring char*
#define ptr void*

#define var auto
#define bitfield(i) _BitInt(i)

#define object(name) \
    typedef struct name name;\
    struct name

#define option(name) \
    typedef enum name name;\
    enum name

#endif // _C4_TYPES_H
