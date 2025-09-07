#include "Store.h"

extern ExceptionHandler exceptionHandler;

Store newStore () {
    return (Store) { .size = 0, .firstItem = NULL };
}

_Boxed* _newStoreItem (Store* store, ptr item, natural size) {
    if (store == NULL) {
        throwException(NULL_STORE_EX);
    }

    if (item == NULL) {
        throwException(NULL_VALUE_EX);
    }

    StoreItem* newItem = malloc(sizeof(StoreItem));

    if (newItem == NULL) {
        throwException(ITEM_ALLOC_EX);
    }

    _Boxed* data = _new(size, item);

    if (data == NULL || (*data).value == NULL) {
        throwException(ITEM_ALLOC_EX);
    }

    (*newItem).data = data;

    if (
        ((*store).firstItem == NULL && (*store).size != 0) ||
        ((*store).firstItem != NULL && (*store).size == 0)
    ) {
       throwException(INVALID_STORE_STATE_EX);
    }

    (*newItem).next = (*store).firstItem;
    (*store).firstItem = newItem;
    (*store).size++;

    return data;
}

_Boxed* _getStoreItem (Store* store, natural index) {
    if (store == NULL) {
        throwException(NULL_REFERENCE_EX);
    }

    StoreItem* next = (*store).firstItem;

    if (index > (*store).size) {
        throwException(INVALID_INDEX_EX);
    }

    for (int i = 0; i < index; ++i) {
        if (next == NULL) {
            throwException(INVALID_INDEX_EX);
        }

        next = (*next).next;
    }

    if (next == NULL) {
        throwException(INVALID_INDEX_EX);
    }

    if ((*next).data == NULL) {
        throwException(NULL_REFERENCE_EX);
    }

    return (*next).data;
}

boolean deleteStore (Store* store) {
    if (store == NULL) {
        return false;
    }

    while ((*store).size > 0 && (*store).firstItem != NULL) {
        StoreItem* next = (*store).firstItem;

        if ((*next).data != NULL) {
            delete((*next).data);
        }

        free(next);

        (*store).firstItem = (*next).next;
        (*store).size--;
    }

    return true;
}
