/*
 * ringbuffer_02.h - yet another version of ringbuffer_t optimized for msp430-gcc
 *
 * Desc: This template class creates a ringbuffer with arbitrary types. Provides push
 *       and pop methods for adding and removing items.  Access function for checking
 *       the number of items in the buffer, capacity and full or empty methods
 *       provide safe access to the info.
 *
 *       This version has been optimized for the msp430-gcc. It doesn't use disable
 *       or enable any interrupts. It is safe nonetheless for use when there is a single
 *       writer and single reader. This is common when using an interrupt and the main
 *       line thread working together to move data in and out of peripherals on demand.
 *
 * Version: 1.0.0
 * Created: Jul-24-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 02-28-2013
 * Version: 1.0.0
 *
 * =========================================================================
 *  Copyright © 2012 Rick Kimball
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef RINGBUFFER_T_H_
#define RINGBUFFER_T_H_
#include <stdint.h>

/**
 * uint16x2_t - a union containing 16 bit head and tail offsets into the ring buffer.
 *              The union make it simpler to grab both values with asm.
 *
 */
union uint16x2_t {
    // access as 32 bit
    unsigned long both;
    // -- or as 2 16 bit values --
    struct {
        unsigned head:16;
        unsigned tail:16;
    };
};

/**
 * uint8x2_t - a union containing 2 8 bit values that can be accessed a byte
 *             at a time or a word at a time. The union make it more
 *             efficient to grab both values.
 *
 */
typedef union uint8x2_t {
  //--- word access
  uint16_t as_u16;     // access both as a word: mask is low byte, data is high byte
  //--- or --- individual byte access
  struct {
    uint8_t as_u8[2];
  };
} uint8x2_t;

/**
 * ringbuffer_t - provide a circular_buffer without disabling interrupts
 *    expects a power of 2 container, and only one reader and one writer
 *    container capacity SIZE-1
 */

template <
    typename T,                     /* works with any type */
    uint16_t SIZE,                  /* how many elements-1 must be power of 2 */
    typename POP_T = int16_t,       /* return type of pop_front */
    POP_T EMPTY_ELEM = (POP_T)-1    /* default return value when empty */
    >
struct ringbuffer_t {

    // --- private structure data ---
    volatile uint16x2_t offsets;
    T elements[SIZE];
    enum { CAPACITY=SIZE-1 };

    is_power_of_two<SIZE> check_buffer_size; // your SIZE is not a power of 2, if you error here

public:
    // --- methods ---

    inline void clear(void ) {
        offsets.both=0;
    }

    ALWAYS_INLINE size_t available() {
        register uint16x2_t temp = { offsets.both };

        temp.both = (temp.head-temp.tail) & CAPACITY;

        return temp.both;
    }

    ALWAYS_INLINE size_t capacity() {
        return CAPACITY;
    }

    bool inline empty() {
        return !available();
    }

    bool inline full() {
        return available() == capacity();
    }

    // affects head, reads tail, element ignored if overflow ~1.68 us @16MHz
    inline void push_back(const T element) {
        register uint16_t temp_head = offsets.head;

        elements[temp_head++] = element;
        temp_head &= CAPACITY;
        if ( !(temp_head == offsets.tail) ) { // !full
            offsets.head = temp_head;
        }

        return;
    }

    // no bounds check version, affects head ~1.120 us @ 16MHz
    inline void push_back_nc(const T element) {
        register uint16_t temp_head = offsets.head;

        elements[temp_head++] = element;
        offsets.head = temp_head & CAPACITY;
        return;
    }


    // affects tail, reads head  ~1.80 us @ 16MHz
    inline POP_T pop_front(void) {
        register uint16x2_t temp = { offsets.both };

        if ( (temp.head-temp.tail) & CAPACITY ) { // !empty
            POP_T elem = elements[temp.tail++];
            offsets.tail = temp.tail & CAPACITY;
            return elem;
        }

        return EMPTY_ELEM; // on underflow return default element
    }

    // no bounds check, affects tail, reads head ~1.128 us @ 16MHz
    inline POP_T pop_front_nc(void) {
        register uint16_t temp_tail = offsets.tail;

        POP_T elem = elements[temp_tail++];
        offsets.tail = temp_tail & CAPACITY;
        return elem;
    }

};

#endif /* RINGBUFFER_T_H_ */
