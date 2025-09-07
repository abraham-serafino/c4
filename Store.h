#ifndef _C4_STORE_H
#define _C4_STORE_H

#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "Exception.h"
#include "strings.h"
#include "ref.h"

object (StoreItem) {
    natural size;
    boolean isNull;
    _Boxed* data;
    struct StoreItem* next;
};

object (Store) {
    natural size;
    StoreItem* firstItem;
};

Store newStore ();
_Boxed* _newStoreItem (Store* store, ptr item, natural size);
_Boxed* _getStoreItem (Store* store, natural index);
boolean deleteStore (Store* store);

#define newStoreItem(store, T, item) _newStoreItem(store, item, sizeof(T))
#define getStoreItem(store, index) _getStoreItem(store, index)

#endif // _C4_STORE_H
