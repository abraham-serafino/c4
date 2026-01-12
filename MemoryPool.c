#ifndef _C4_MEMORY_POOL_
#define _C4_MEMORY_POOL_

#include "types.c"
#include "loops.c"
#include "C4Functions.c"
#include <limits.h>

#define ErrorCodeMap(X) \
    X(Success,                  0)  \
    X(InvalidPool,              1)  \
    X(PageOverflow,             2)  \
    X(RequiredFunctionsMissing, 3)

defineStaticMap(ErrorCodeMap, ErrorCodes);

#define ALIGN(value) (                  \
     ((value) + (alignof(void*)) - 1) & \
    ~((alignof(void*)) - 1)             \
)

#define TOTAL_PAGE_SIZE(pageSize)       \
    sizeof(MemoryPage) + (pageSize)

typedef struct MemoryPool MemoryPool;

object              (MemoryPage) {
    uint            offset;
    uint            generation;
    MemoryPage*     next;
    byte            data[];
};

object              (MemoryPool) {
    MemoryPool*     parentPool;
    MemoryPage*     currentPage;
    MemoryPage*     firstFreePage;
    bool            autoResize;
    uint            pageCount;
    uint            pageSize;
    C4Functions*    functions;
};

object              (MemoryPoolOptions) {
    MemoryPage*     firstPage;
    MemoryPool*     parentPool;
    bool            autoResize;
    uint            pageCount;
    uint            pageSize;
    C4Functions*    functions;
};

MemoryPage* reservePages (
    MemoryPool* pool,
    uint        pageCount
) {
    if (pool == nullptr) {
        return  nullptr;
    }

    var firstPage        = (MemoryPage*) nullptr;

    var totalSize = ALIGN(
        TOTAL_PAGE_SIZE((*pool).pageSize)
    );

    if (totalSize == 0) {
        return nullptr;
    }

    if (
        (*pool).functions != nullptr &&
        (*(*pool).functions).allocate != nullptr
    ) {
        var region = (*(*pool).functions).allocate(
            pageCount * ALIGN(
                TOTAL_PAGE_SIZE((*pool).pageSize)
            )
        );

        if (region == nullptr) {
            return nullptr;
        }

        count_down (from(pageCount - 1), to(0), as(i)) {
            ((MemoryPage*) region)[i] = (MemoryPage) {
                .next = firstPage
            };

            firstPage = &((MemoryPage*) region)[i];
        }

        if (firstPage == nullptr) {
            return nullptr;
        }

    } else if ((*pool).firstFreePage != nullptr) {
        firstPage = (*pool).firstFreePage;
        var nextPage = firstPage;

        count_up (from(0), to(pageCount), as(i)) {
            if (nextPage == nullptr) {
                return nullptr;
            }

            nextPage = (*nextPage).next;
        }

        (*pool).firstFreePage = nextPage;
    }

    (*firstPage).offset = 0;
    return       firstPage;
}

#define reservePage(pool) \
    reservePages(pool, 1)

MemoryPool _createMemoryPool (MemoryPoolOptions options) {
    var firstPage   = (MemoryPage*) nullptr;
 
    if (options.firstPage) {
        firstPage   = options.firstPage;

    } else if (options.parentPool) {
        firstPage   = reservePages(
            options.parentPool,
            options.pageCount
        );

    } else if (
        options.functions                           == nullptr ||
        (*options.functions).setMemory              == nullptr ||
        (*options.functions).generateUniqueNumber   == nullptr
    ) {
        throwException(ErrorCodes.RequiredFunctionsMissing);
        return (MemoryPool) {};
    }

    if (! options.pageSize || ! options.pageCount) {
        return (MemoryPool) {};
    }

    var result  = (MemoryPool) {
        .firstFreePage      = firstPage,
        .autoResize         = options.autoResize,
        .parentPool         = options.parentPool,
        .pageCount          = options.pageCount,
        .pageSize           = options.pageSize,
        .functions          = options.functions,
    };

    return result;
}

#define createMemoryPool(...) \
    _createMemoryPool ((MemoryPoolOptions) { __VA_ARGS__ })

object (PoolItem) {
    uint        generation;
    byte        data[];
};

object (PoolHandle) {
    uint        generation;
    PoolItem*   item;
};

#define POOL_ITEM_HEADER_SIZE ALIGN(sizeof(PoolItem))

object          (ReserveBytesOptions) {
    MemoryPool* pool;
    uint        size;
    uint        count;
    bool        setToZero;
};

PoolHandle _reservePoolBytes (ReserveBytesOptions options) {
    if (options.pool == nullptr) {
      throwException(ErrorCodes.InvalidPool);
      return (PoolHandle) {};
    }

    if (options.size == 0) {
      return (PoolHandle) {};
    }

    var pageSize = (*options.pool).pageSize;
    var size     = options.size;  

    if (size > pageSize) {
        throwException(ErrorCodes.PageOverflow);
        return (PoolHandle) {};
    }

    var pool            = options.pool;
    var currentPage     = (*pool).currentPage;
    var offset          = ALIGN((*currentPage).offset);

    var count               = (options.count > 0) ?
                                     options.count :
                                     1;

    var totalSize       = ALIGN(
        POOL_ITEM_HEADER_SIZE + (size * count)
    );

    if (
        (currentPage == nullptr || (pageSize - offset) < totalSize) &&
        (*pool).autoResize
    ) {
        currentPage             = reservePage(pool);

        if (currentPage == nullptr) {
            offset              = 0;
            (*pool).currentPage = currentPage;
        }
    }

    if (currentPage == nullptr) {
        return (PoolHandle) {};
    }

    var item        = (PoolItem*) &((*currentPage).data[offset]);

    var generation  = __atomic_add_fetch(
        &(*currentPage).generation,
        1,
        __ATOMIC_SEQ_CST
    );

    (*(PoolItem*) item) .generation = generation;
    (*currentPage)      .offset     += totalSize;

    if (
        options.setToZero                           &&
        (*pool).functions               != nullptr  &&
        (*(*pool).functions).setMemory  != nullptr
    ) {

        (*(*pool).functions).setMemory(
            & (*item).data,
            0,
            totalSize
        );
    }

    return (PoolHandle) {
        .generation     = generation,
        .item           = item
    };
}

#define PoolPointer(T, name)    \
    object (name) {             \
        PoolHandle  handle;     \
        T           _type[];    \
    };

#define reservePoolBytes(T, ...) (                      \
    (T) {                                               \
        .handle                                         \
            = _reservePoolBytes((ReserveBytesOptions) { \
                .size = sizeof(((T*) 0)->_type[0]),     \
                __VA_ARGS__                             \
            })                                          \
    }                                                   \
)

bool returnPages (
    MemoryPool* pool,
    MemoryPage* firstPage
) {
    if (pool == nullptr) {
        return false;
    }

    var page    = firstPage;
    var result  = true;

    until (page == nullptr) {
        var nextPage = (*page).next;

        if (
            (*pool).functions != nullptr                &&
            (*(*pool).functions).deallocate != nullptr
        ) {
            result = (*(*pool).functions)
                .deallocate((void**) &page);

        } else {
            (*page).next = (*pool).firstFreePage;
            (*pool).firstFreePage = page;
        }

        page = nextPage;
    }

    return result;
}

bool clearMemoryPool (MemoryPool* pool) {
    if (pool == nullptr) {
        return false;
    }

    if (
        (*pool).parentPool                  != nullptr      || (
            (*pool).functions               != nullptr      &&
            (*(*pool).functions).deallocate != nullptr
        )
    ) {
        return returnPages(pool, (*pool).currentPage);
    }

    until ((*pool).currentPage == nullptr) {
        var page = (*pool).currentPage;
        (*pool).currentPage    = (*page).next;
        (*page).next           = (*pool).firstFreePage;
        (*pool).firstFreePage  = page;
    }

    var firstFreePage           = reservePage(pool);

    if (firstFreePage != nullptr) {
        (*pool).currentPage     = firstFreePage;
    }

    return true;
}

#endif // _C4_MEMORY_POOL_
