;;;; ErrorCodes.dan ;;;;

enum ErrorCode {
    Success,
    InvalidPool,
    PageOverflow,
    RequiredFunctionsMissing,
    InvalidHandle,
    NullReference
}

;;;; GuidGenerator.dan ;;;;

type GuidGenerator = fn () -> uint64

fn generateGuid () -> uint64 {
    static counter = (uint64) 0

    counter += 1
    return counter
}

;;;; Types.c ;;;;

#include <stddef.h>
typedef size_t C4_Types_ArraySize;


;;;; Functions.dan ;;;;

module C4           from "./C4.dan"
use { ArraySize }   from C4.Types

type Allocator = fn (bytes = (ArraySize) 0) -> ref

type Reallocator = fn (
    ref memoryAt    = null,
    toHave          = (ArraySize) 0
) -> ref

type Deallocator = fn (
    ref ref dataAt = null ; a reference to a reference
) -> bit

type MemorySetter = fn (
    ref at      = null,                 ; untyped references default to void*
    toValue     = (int32) 0,
    withSize    = (ArraySize) 0
) -> ref

object Functions {
    allocate    = (Allocator) null,     ; Any time a function is stored in
    allocate0   = (Allocator) null,     ; memory, we are really storing a
    deallocate  = (Deallocator) null,   ; "reference" (ie, a pointer) to it
    nextGuid    = (GuidGenerator) null  ; under the hood.
}


;;;; C4.dan ;;;;

module Functions    from "./Functions.dan"
module Types        from "./types.c"
module ErrorCodes   from "./ErrorCodes.dan"
module MemoryPool   from "./MemoryPool.dan"


;;;; MemoryPool.dan ;;;;

module C4                           from "./C4.dan"
use { ErrorCode }                   from C4.ErrorCodes
use { ArraySize }                   from C4.Types

object MemoryPage {

    ;; untyped, positive integer values default to uint32
    offset      = 0,

    generation  = 0,
    ref next    = (MemoryPage) null,

    ;; an uninitialized 'byte' array of unknown size
    data        = byte[]
}

object MemoryPool {
    ref parentPool      = (MemoryPool) null,
    ref currentPage     = (MemoryPage) null,
    ref firstFreePage   = (MemoryPage) null,
    ref functions       = (C4.Functions) null,

    autoResize          = false, ; 'true' and 'false' are 'bit' values
    pageCount           = 0,
    pageSize            = 0
}

object PoolItem of T {
    pageGeneration = (uint64) 0,
    data = T[]
}

object PoolHandle of T {
    pageGeneration  = (uint64) 0,
    ref page        = (MemoryPage) null,
    ref item        = (PoolItem of T) null
}

fn init (
    ref pool                                = (MemoryPool) null,
    ref fromMemorySource:   memorySource    = (byte) null,
    ref withParentPool:     parentPool      = (MemoryPool) null,
    ref usingFunctions:     functions       = (C4.Functions) null,
    withPageCount:          pageCount       = 0,
    withPageSize:           pageSize        = (ArraySize) 0,
    setAutoResize:          autoResize      = false
) -> {

    if (pool.isNull()) { throw ErrorCode.InvalidPool }

    if (functions?.nextGuid.isNull()) {
        throw ErrorCode.RequiredFunctionsMissing
    }

    unless (pageSize > 0 and pageCount > 0) return {}
    ref firstPage = (MemoryPage) null

    unless (memorySource.isNull()) {
        firstPage = split(
            memorySource,
            intoPages:  pageCount,
            ofSize:     pageSize,
            using:      functions.nextGuid
        )

    } otherwise,
    unless (functions?.allocate.isNull()) {

        pool.functions = functions
        pool.pageSize = pageSize
        firstPage = reserve(pages: pageCount, fromPool: pool)

    } otherwise,
    unless (parentPool.isNull()) {

        firstPage = reserve(pages: pageCount, fromPool: parentPool)
    }

    pool = {
        firstFreePage: firstPage,
        autoResize,
        memorySource,
        parentPool,
        pageCount,
        pageSize,
        functions
    }
}

fn reserve(
    pages:          pageCount   = (ArraySize) 0
    ref fromPool:   pool        = (MemoryPool) null,

) -> ref MemoryPage {                       ; return types cannot be implied

    if (pool.isNull()) { return null }

    ref firstPage = (MemoryPage) null
    var totalSize = @TOTAL_PAGE_SIZE(fromOriginal: pool.pageSize)

    if (totalSize == 0) { return null }

    unless (pool.functions?.allocate.isNull()) {
        if (pool.functions.nextGuid.isNull()) {
            throw ErrorCode.RequiredFunctionsMissing
        }

        ref region = (MemoryPage) pool.functions.allocate (
            pageCount * @align(TOTAL_PAGE_SIZE(fromOrigin: pool.pageSize))
        )
    
        if (region.isNull()) { return null }

        firstPage = split(
            region,
            intoPages:  pageCount,
            ofSize:     pool.pageSize,
            using:      pool.functions.nextGuid
        )

    } otherwise,
    unless (pool.firstFreePage.isNull()) {

        firstPage       = pool.firstFreePage
        ref nextPage    = firstPage

        for (i from 1 <= pageCount) {
            if (nextPage.isNull()) { return null }
            nextPage = nextPage.next
        }

        pool.firstFreePage = nextPage.next
        nextPage.next = null
    }

    ; The following is syntactic sugar for:
    ; if (firstPage.isNull()) { firstPage.offset = 0 }

    firstPage?.offset = 0
    return firstPage
}

fn returnAll (
    ref pages:  firstPage   = (MemoryPage) null,
    ref toPool: pool        = (MemoryPool) null
) -> boolean {

    if (pool.isNull()) { return false }
    if (pool.functions?.nextGuid.isNull()) { throw RequiredFunctionsMissing }

    ref page = firstPage

    unless (pool.functions?.deallocate.isNull()) {
        until (page.isNull()) {
            ref nextPage = page.next
            page.generation = pool.functions.nextGuid()

            unless (pool.functions.deallocate(
                dataAt: addressOf (page)
            )) {
                return false
            }

            page = nextPage
        }

    } otherwise,
    until (page.isNull()) {

        ref nextPage = page.next
        page.generation = pool.functions.nextGuid()

        page.next = pool.firstFreePage
        pool.firstFreePage = page

        page = nextPage
    }

    return true
}

fn clear (
    memoryPool = (MemoryPool) null
) -> boolean {

    if (memoryPool.isNull()) { return false }

    unless (
        memoryPool.parentPool.isNull() or
        memoryPool.functions?.deallocate.isNull()
    ) {
        return returnAll(pages: currentPage, toPool: memoryPool)
    }
 
    until (pool.currentPage.isNull()) {
        ref page = pool.currentPage
        pool.currentPage = page.next
        page.next = pool.firstFreePage
        pool.firstFreePage = page
    }

    return true
}

fn reserve (
    bytes                   = (ArraySize) 0,
    ref fromPool:   pool    = (MemoryPool) null,
    times:          count   = 0,
    setToZero               = false
) -> PoolHandle of T {

    if (pool.isNull()) { throw ErrorCode.InvalidPool }

    if (functions?.nextGuid.isNull()) {
        throw ErrorCode.RequiredFunctionsMissing
    }

    if (bytes == 0) { return {} }

    ; Unlike Javascript, destructuring does not result in a copy.
    ; Instead, it's like a limited "using" or "with" clause, that auto-
    ; expands 'pageSize' to 'pool.pageSize', 'currentPage' to
    ; 'pool.currentPage', etc.

    var { pageSize, currentPage, autoResize, functions } = pool

    if (size > pageSize) { throw ErrorCode.PageOverflow }

    var offset = @align(currentPage.offset)
    var byteCount = (count > 0) ? count : 1

    var totalSize = @align(
        @align(sizeof(PoolItem)) + (size * byteCount)
    )

    if (
        autoResize and
        (currentPage.isNull() or (pageSize - offset) < totalSize)
    ) {
        currentPage = reserve(pages: 1, fromPool: pool)
        if (currentPage.isNull()) { return {} }

        currentPage.offset = 0
        pool.currentPage = currentPage
    }

    ref item = (PoolItem of T) addressOf (currentPage.data[offset])
    item.pageGeneration = currentPage.generation

    if (setToZero and pool.functions?.setMemory.isNull()) {

        pool.functions.setMemory(
            at:         addressOf (item.data),
            withSize:   size,
            to:         0
        )
    }

    currentPage.offset += totalSize

    return {
        pageGeneration: currentPage.generation,
        page:           currentPage,
        item
    }
}

fn @unbox(
    handle = (PoolHandle of T) {}
) -> ref T {

    if (handle.page.isNull()) or handle.item.isNull() {
        throw ErrorCode.InvalidHandle

    } otherwise, if (
        handle.pageGeneration != handle.page.generation or
        handle.pageGeneration != handle.item.pageGeneration
    ) {

        throw ErrorCode.InvalidHandle
    }

    resolve handle.item.data
}

private fn split(
    ref region              = (byte) null,
    intoPages:  pageCount   = 0,
    ofSize:     pageSize    = (ArraySize) 0,
    using:      nextGuid    = (GuidGenerator) null
) -> ref MemoryPage {

    if (region.isNull()) { throw ErrorCode.NullReference }
    if (nextGuid.isNull()) { throw ErrorCode.RequiredFunctionsMissing }

    var totalPageSize   = @TOTAL_PAGE_SIZE(fromOriginal: pageSize)
    ref firstPage       = (MemoryPage) null
    ref raw             = region

    for (i from (pageCount - 1) >= 0) {
        ref next = (MemoryPage) (raw + (i * totalPageSize))

        ;; Dereferencing is implied by literal primitive assignment.
        next = MemoryPage { next: firstPage }

        ; The data pointed to by next has now been updated to:
        ; (MemoryPage) { next: firstPage }


        ;; Address copying is indicated by assigning one ref to another.
        firstPage = next

        ; firstPage now points to the same memory address as next.
    }

    return firstPage
}

; @align is the following, "builtin" C macro:

; typedef struct s_ { char c; void* p; } s_;
; #define ALIGNED_POINTER offsetof(s_, p)
; #define align(value) \
;    (((value) + (ALIGNED_POINTER) - 1) & ~(ALIGNED_POINTER) - 1)

private fn @TOTAL_PAGE_SIZE(
    fromOriginal: pageSize = 0
) -> uint32 {

    resolve @align(sizeof(MemoryPage) + @align(pageSize))
}
