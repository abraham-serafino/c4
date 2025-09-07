#ifndef _C4_REF_H
#define _C4_REF_H

#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "Exception.h"
#include "strings.h"

object (_Boxed) {
    void* value;
    natural size;
};

#define boxed(T) _Boxed*

typedef boxed(int8) Int8;
typedef boxed(byte) Byte;
typedef boxed(boolean) Boolean;
typedef boxed(int16) Int16;
typedef boxed(word) Word;
typedef boxed(int) Int;
typedef boxed(uint) Natural;
typedef boxed(number) Number;
typedef boxed(ulong) ULong;
typedef boxed(superlong) Superlong;

_Boxed* _new(natural size, void* value);
void* _deref (_Boxed* wrapper, natural size);
void delete (_Boxed* wrapper);

#define new(T, value) _new(sizeof(T), &((T) value))
#define $(T, wrapper) (*((T*) _deref(wrapper, sizeof(T))))
#define isNull(wrapper) ((wrapper == NULL) || (*wrapper).value == NULL)

#endif // _C4_REF_H
