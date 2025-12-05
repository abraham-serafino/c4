#include "ArrayList.h"

ListItem _new (uint size, void* value) {
    ListItem item = {
        .size = size,
        .value = malloc(size)
    };

    if (item.value == NULL) {
        throwException("\nUnable to reserve memory for item.\n");
    }

    memcpy(item.value, value, size);
    return item;
}

void* _unbox (ListItem* wrapper, uint size) {
    if (wrapper == NULL || (*wrapper).value == NULL) {
        throwException("\nNull reference exception.\n");
    }

    if ((*wrapper).size != size) {
        throwException("\nItem is not the size you are expecting.\n");
    }

    return (*wrapper).value;
}


void delete (ListItem* item) {
    if (wrapper == NULL) {
        return;
    }

    if ((*item).value != NULL) {
        free((*item).value);
    }

    (*item).value = NULL;
}

ArrayList newArrayList (uint capacity) {
    var result = (ArrayList) { .capacity = capacity, .length = 0 };
    result.items = malloc(sizeof(ListItem*) * capacity);

    if (result.items == NULL) {
        throwException("\nUnable to reserve memory for list.\n");
    }

    result.items[0] = NULL;
    result.length = 0;
    result.capacity = capacity;

    return result;
}

ListItem* insertItem (ArrayList* list, ListItem item, uint index) {
    if (list == NULL) {
        throwException("\nNull reference exception.\n");
    }

    if (item == NULL) {
        throwException("\nCannot insert a NULL item.\n");
    }

    if (index > (*list).length) {
        throwException("\nInvalid index.\n");
    }

    var capacity = (*list).capacity;
    var newLength = (*list).length + 1;

    if (newLength > capacity) {
        var newCapacity = newLength * 2;
        if ((*list).items == NULL) {
            (*list).items = malloc(sizeof(_Boxed*) * newCapacity);

        } else {

            (*list).items = realloc(
                (*list).items,
                sizeof(_Boxed*) * newCapacity
            );
        }

        if ((*list).items == NULL) {
            throwException("\nUnable to reserve memory for new list item.\n");
        }

        (*list).capacity = newCapacity;
    }

    var currentIndex = (*list).length;

    while (currentIndex > index) {
        (*list).items[currentIndex] = (*list).items[currentIndex - 1];
        currentIndex--;
    }

    (*list).items[index] = item;
    (*list).length = newLength;

    return &((*list).items[index]);
}

ListItem* getItem (ArrayList* list, uint index) {
    if (list == NULL) {
        throwException("\nNull reference exception.\n");
    }

    if (index >= (*list).length) {
        throwException("\nInvalid index.\n");
    }

    return (*list).items[index];
}

ListItem removeItem (ArrayList* list, uint index) {
    var item = getItem(list, index);
    var length = (*list).length - index;

    memcpy(
        &((*list).items[index]),
        &((*list).items[index + 1]),
        sizeof(ListItem*) * length
    );

    (*list).length--;

    return (*item);
}

boolean isEmpty (ArrayList* list) {
    if (list == NULL || (*list).length <= 0 || (*list).items == NULL) {
        return true;

    } else {
        return false;
    }
}

void deleteList (ArrayList* list) {
    if (list != NULL && (*list).items != NULL) {
        var index = 0;
        var listLength = (*list).length;

        while (index < listLength) {
            delete((*list).items[index]);
            (*list).length--;
            index++;
        }

        free((*list).items);
        (*list).items = NULL;
        (*list).capacity = 0;
    }
}


