#include <stdbool.h>
#include <errno.h>
#include <stdint.h>

extern uint32_t __mbed_sbrk_start;
extern uint32_t __mbed_krbs_start;
extern uint32_t __mbed_sbrk_start_1;
extern uint32_t __mbed_krbs_start_1;

extern uint32_t _sbrk_case;
extern int _sbrk_incr;
extern unsigned char *_sbrk_prev_heap;
extern unsigned char *_sbrk_new_heap;

void *_sbrk(int incr)
{
    static uint32_t heap_ind = (uint32_t) &__mbed_sbrk_start;
    static bool once = true;
    uint32_t heap_ind_old = heap_ind;
    uint32_t heap_ind_new = heap_ind_old + incr;

    _sbrk_incr = incr;
    _sbrk_prev_heap = (unsigned char *)heap_ind_old;
    _sbrk_new_heap = (unsigned char *)heap_ind_new;

    /**
     * If the new address is outside the first region, start allocating from the second region.
     */
    do {
        _sbrk_case = 5;
        if (once && (heap_ind_new > (uint32_t) &__mbed_krbs_start)) {
            once = false;
            heap_ind = (uint32_t)&__mbed_sbrk_start_1;
            heap_ind_old = heap_ind;
            heap_ind_new = heap_ind_old + incr;
            _sbrk_case = 1;
            _sbrk_prev_heap = (unsigned char *)heap_ind_old;
            _sbrk_new_heap = (unsigned char *)heap_ind_new;
            continue;
        }
        if (once && (heap_ind_new == (uint32_t) &__mbed_krbs_start)) {
            once = false;
            heap_ind_new = (uint32_t)&__mbed_sbrk_start_1;
            _sbrk_case = 2;
        } else if (heap_ind_new > (uint32_t) &__mbed_krbs_start_1) {
            _sbrk_case = 3;
            errno = ENOMEM;
            return (void *) - 1;
        }
    } while (0);
    heap_ind = heap_ind_new;

    return (void *) heap_ind_old;
}
