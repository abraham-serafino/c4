#include "ref.h"

extern ExceptionHandler exceptionHandler;

_Boxed* _new(natural size, ptr value) {
    if (value == NULL) {
        throwException(NULL_VALUE_EX);
    }

    _Boxed* wrapper = malloc(sizeof(_Boxed));

    if (wrapper == NULL) {
        throwException(ITEM_ALLOC_EX);
    }

    (*wrapper).value = malloc(size);

    if ((*wrapper).value == NULL) {
        throwException(ITEM_ALLOC_EX);
    }

    memcpy((*wrapper).value, value, size);
    (*wrapper).size = size;

    return wrapper;
}

void* _unbox (_Boxed* wrapper, natural size) {
    if (wrapper == NULL) {
        throwException(NULL_REFERENCE_EX);
    }

    if ((*wrapper).value == NULL) {
        throwException(NULL_REFERENCE_EX);
    }

    if ((*wrapper).size != size) {
        throwException(ITEM_SIZE_EX);
    }

    return (*wrapper).value;
}

void delete (_Boxed* wrapper) {
    if (wrapper == NULL) {
        return;
    }

    if ((*wrapper).value != NULL) {
        free((*wrapper).value);
    }

    (*wrapper).value = NULL;
}

