#ifndef _STORE_H
#define _STORE_H

#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "Exception.h"

object (StoreItem) {
    natural size;
    boolean isNull;
    ptr data;
    struct StoreItem* next;
};

object (Store) {
    natural size;
    StoreItem* firstItem;
};

Store newStore ();
StoreItem* _newStoreItem (Store* store, ptr item, natural size);
StoreItem* _getStoreItem (Store* store, natural index, natural size);
ptr _unbox (StoreItem* item, natural size);
boolean deleteStore (Store* store);

#define newStoreItem(T, store, item) _newStoreItem(store, item, sizeof(T))
#define getStoreItem(T, store, index) _getStoreItem(store, index, sizeof(T))
#define unbox(T, item) _unbox(item, sizeof(T))

#endif // _STORE_H
