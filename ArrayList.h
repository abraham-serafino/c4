#ifndef _C4_ARRAY_LIST_H_
#define _C4_ARRAY_LIST_H_

#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "Exception.h"

object (ListItem) {
    uint size;
    void* value;
}

object (ArrayList) {
    uint length;
    uint capacity;
    ListItem* items;
};

ListItem _new (uint size, void* value) {
void* _unbox (uint size, ListItem* wrapper);
void delete (ListItem* item);

ArrayList newArrayList (uint capacity);
void deleteList (ArrayList* list);
_Boxed* insertItem (ArrayList* list, uint size, void* value, uint index);
_Boxed* getItem (ArrayList* list, uint index);
_Boxed* removeItem (ArrayList* list, uint index);
boolean isEmpty(ArrayList* list);

#define new(T, value) _new(sizeof(T), &((T) value))
#define unbox(T, wrapper)  (*((T*) _unbox(sizeof(T), wrapper)))

#define insertItem(list, T, index) \
    _insertItem(list, sizeof(T), index)

#define shiftItemIn(list, T) insertItem(list, T, 0);
#define shiftItemOut(list) removeItem(list, 0);

#define pushItem(list, T) insertItem(list, T, (*(list)).length);
#define popItem(list) getItem(list, (*(list)).length);

#endif // _C4_ARRAY_LIST_H_

