#include "Store.h"
#include "Store.strings.h"

extern ExceptionHandler exceptionHandler;

void throwException (const cstring message) {
    if (message != NULL && exceptionHandler != NULL) {

        exceptionHandler((Exception) {
            .message = message
        });
    }
}

Store newStore () {
    return (Store) { .size = 0, .firstItem = NULL };
}

StoreItem* _newStoreItem (Store* store, ptr item, natural size) {
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

    (*newItem).data = malloc(size);

    if ((*newItem).data == NULL) {
        throwException(ITEM_ALLOC_EX);
    }

    memcpy((*newItem).data, item, size);
    (*newItem).size = size;

    if (
        ((*store).firstItem == NULL && (*store).size != 0) ||
        ((*store).firstItem != NULL && (*store).size == 0)
    ) {
       throwException(INVALID_STORE_STATE_EX);
    }

    (*newItem).isNull = false;
    (*newItem).next = (*store).firstItem;
    (*store).firstItem = newItem;
    (*store).size++;

    return newItem;
}

StoreItem* _getStoreItem (Store* store, natural index, natural size) {
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

    if ((*next).isNull) {
        throwException(NULL_REFERENCE_EX);
    }

    if ((*next).size != size) {
        throwException(ITEM_SIZE_EX);
    }

    return next;
}

ptr _unbox (StoreItem* item, natural size) {
    if (item == NULL || (*item).isNull || (*item).data == NULL) {
        throwException(NULL_REFERENCE_EX);
    }

    if ((*item).size != size) {
        throwException(ITEM_SIZE_EX);
    }

    return (*item).data;
}

boolean deleteStore (Store* store) {
    if (store == NULL) {
        return false;
    }

    while ((*store).size > 0 && (*store).firstItem != NULL) {
        StoreItem* next = (*store).firstItem;

        if ((*next).data != NULL) {
            free((*next).data);
        }

        (*next).isNull = true;
        (*next).data = NULL;

        (*store).firstItem = (*next).next;
        (*store).size--;
    }

    return true;
}
