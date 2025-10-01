#ifndef _C4_ARRAY_LIST_H_
#define _C4_ARRAY_LIST_H_

#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "Exception.h"
#include "ref.h"

object (ArrayList) {
    uint length;
    uint capacity;
    _Boxed** items;
};

ArrayList newArrayList (uint capacity);
void deleteList (ArrayList* list);
_Boxed* insertItem (ArrayList* list, _Boxed* value, uint index);
_Boxed* getItem (ArrayList* list, uint index);
_Boxed* removeItem (ArrayList* list, uint index);
boolean isEmpty(ArrayList* list);

#define shiftItemIn(list, value) insertItem(list, value, 0);
#define shiftItemOut(list) removeItem(list, 0);

#define pushItem(list, value) insertItem(list, value, (*(list)).length);
#define popItem(list) getItem(list, (*(list)).length);

#endif // _C4_ARRAY_LIST_H_

