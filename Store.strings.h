#ifndef _STORE_STRINGS_H
#define _STORE_STRINGS_H

const cstring NULL_STORE_EX = "\nCannot create item for NULL store.\n";

const cstring NULL_VALUE_EX =
    "\nNull value cannot be used to create store item.\n";

const cstring ITEM_ALLOC_EX =
    "\nMemory error: Unable to reserve space for new store item.\n";

const cstring INVALID_STORE_STATE_EX = "\n"
    "Store is in an invalid state.\n\n"
    "Please do not modify the Store without using the designated\n"
    "utility functions.\n";

const cstring GET_NULL_ITEM_EX =
    "\nCannot get item from NULL store.\n";

const cstring INVALID_INDEX_EX = "\nInvalid index.\n";

const cstring NULL_REFERENCE_EX = "\nNull reference exception!\n";

const cstring ITEM_SIZE_EX =
    "\nThe item you are retrieving is not the size you expected.\n";

#endif // _STORE_STRINGS_H
