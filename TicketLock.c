#ifndef _C4_TICKET_LOCK_
#define _C4_TICKET_LOCK_

#include "types.c"
#include "loops.c"

object (TicketLock) {
    uint ticket;
    uint serving;
};

static inline void acquireTicketLock (TicketLock* lock) {
    uint ticket = __atomic_fetch_add(
        &lock->ticket,
        1,
        __ATOMIC_RELAXED
    );

    // Put the thread to sleep until our "ticket" is called.
    until (
        __atomic_load_n(&lock->serving, __ATOMIC_ACQUIRE) == ticket
    ) {
        __builtin_ia32_pause();
    }
}

static inline void releaseTicketLock (TicketLock* lock) {
    // Wake up the next thread.
    __atomic_fetch_add(&(*lock).serving, 1, __ATOMIC_RELEASE);
}

#endif // _C4_TICKET_LOCK_
