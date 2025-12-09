#ifndef _C4_H_
#define _C4_H_

#include "types.h"
#include "throwException.h"
#include "pointer.h"

#ifndef _C4_NO_DEBUG_
    #include <stdio.h>
    #define debug printf
#else
    #define debug
#endif //_C4_NO_DEBUG_

#define from(x) x

#define forEach(i, x, ...) for (int i = x; i __VA_ARGS__ i)
#define upTo(x) < (x); ++
#define downTo(x) >= (x); --

#define loop_until(...) while(! (__VA_ARGS__))
#define unless(...) if(! (__VA_ARGS__))

#endif // _C4_H_
