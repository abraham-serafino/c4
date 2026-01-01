#ifndef _C4_DEFAULTS_
#define _C4_DEFAULTS_

#define MILLISECONDS_PER_SECOND     1000;
#define NANOSECONDS_PER_MILLISECOND 1000000;

ExceptionHandler    exceptionHandler;
Allocator           allocate;
Allocator           allocate0;
Reallocator         reallocate;
Deallocator         deallocate;
MemorySetter        setMemory;

#endif // _C4_DEFAULTS_
