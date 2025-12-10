#ifndef _C4_DEFAULTS_H_
#define _C4_DEFAULTS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdatomic.h>

#include "c4.h"

#define MILLISECONDS_PER_SECOND 1000;
#define NANOSECONDS_PER_MILLISECOND 1000000;

void defaultExceptionHandler (cstring message);
uint64 defaultUniqueGenerator ();

void* gcAllocator (uint size, boolean shouldInitialize);
void* gcReallocator (void* original, uint size);
void gcDeallocator (void** data);

void* stdAllocator (uint size, boolean shouldInitialize);
void* stdReallocator (void* original, uint size);
void stdDeallocator (void** data);

void initializeGcDefaults ();
void initializeStandardDefaults ();

#endif // _C4_DEFAULTS_H_
