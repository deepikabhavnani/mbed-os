
#include <stdbool.h>
#include <errno.h>
#include "stm32l4xx.h"

extern uint32_t __mbed_sbrk_start;
extern uint32_t __mbed_krbs_start;
extern uint32_t __mbed_sbrk_start_1;
extern uint32_t __mbed_krbs_start_1;

void *__sbrk(int incr)
{
    static uint32_t heap_ind = (uint32_t) &__mbed_sbrk_start;
    static bool once = true;
    uint32_t heap_ind_old = heap_ind;
    uint32_t heap_ind_new = heap_ind_old + incr;

    /**
     * If the new address is outside the first region, start allocating from the second region.
     */
    if (once && (heap_ind_new >= (uint32_t)&__mbed_krbs_start)) {
        once = false;
        heap_ind_old = (uint32_t)&__mbed_sbrk_start_1;
        heap_ind_new = heap_ind_old + incr;
    /**
     * If the new address is outside the second region, return out-of-memory.
     */
    } else if (heap_ind_new >= (uint32_t)&__mbed_krbs_start_1) {
        errno = ENOMEM;
        return (void *) - 1;
    }

    heap_ind = heap_ind_new;

    return (void *) heap_ind_old;
}
