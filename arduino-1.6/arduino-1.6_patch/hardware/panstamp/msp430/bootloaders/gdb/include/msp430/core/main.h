/**
 * main.cpp - fabooh default main
 *
 * Desc: initialize clock, call user setup and loop.
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 02-28-2013
 * Version: 1.0.0
 *
 * =========================================================================
 *  Copyright © 2013 Rick Kimball
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

#include "fabooh.h"

#ifndef _MAIN_H_
#define _MAIN_H_

int main(void)
{
#if defined(ENERGIA)
    /* warning: optimal size reduction is achieved with -mdisable-watchdog compile time flag*/
    WDTCTL = WDTPW | WDTHOLD;
#endif

#ifndef CUSTOM_CLOCK_INI
    CPU::init_clock();
#endif
    setup();

    while(1) {
        loop();
    }

    return 0;
}

/*
 * Here we provide a way to shrink the size of the exe if the code
 * doesn't actually use the .data or .bss sections. There is no
 * need to include code used to initialize sections when they have
 * a zero size.
 *
 * Use: $ msp430-size foo.elf to see the size of the .data and .bss sections.
 *
 */

#ifdef SMALL_INIT4
  #define NO_DATA_INIT
  #define NO_BSS_INIT
#endif

extern "C"
{
  #ifndef RAM_START
    #define RAM_START 0x0200
  #endif

  #ifndef STACK_COLOR
    #define STACK_COLOR 0xFEEE
  #endif

  /*
   * define COLOR_STACK to enable this feature.
   *
   * You can mark RAM with a known value say 0xfeee. Then at runtime
   * use mspdebug to examine memory looking for locations that no
   * longer have the value of 0xfeee. Memory in ram that no longer
   * contains the value 0xfeee is an indication that it has been used
   * by a data variable, the heap or as stack memory.
   *
   * You need to write a function that runs prior to the start of
   * main and "colors" ram. Link this function with your code and
   * run it with mspdebug. Use CTRL-C to break after you have
   * exercised all your functions. Then use the eXamine gdb cmd:
   *
   * > x/4xw 0x200
   *
   * Here we are eXamining ram memory starting at the lowest address
   * (0x200 is the first ram location for most g series chips).
   * Each time you press enter after that command, it will show
   * the value of the next 4 words of ram. Press enter repeatedly
   * and skip over the bss and data addresses and then start looking
   * for '0xfeee', that is where the heap ends. Continue looking for
   * 0xfeee until it changes, that address indicates the lowest stack
   * value. The difference between the end of the heap and the lowest
   * stack address is how much free ram you have.
   *
   * Note: If the value 0xfeee is a common value used in your program
   * then you probably want to use another value to color the stack.
   *
   * Note: You can't get the amount of free ram just looking at the
   * static numbers provided by the compiler. You must run your code
   * and exercise all code paths to find out the true amount of memory
   * your application uses.
   */

  #if defined(COLOR_STACK)
    void __low_level_init(void) __attribute__((section(".init3"),naked));
    void __low_level_init() {
     __asm__(
         "\n"
         " mov #__stack,r15\n" // ; start at stack and proceed down to RAM_START\n"
         "1:\n"
         " decd r15            ; double decr, a word at a time\n"
         " mov %1,0(r15)       ; 0xfree - virgin untouched stack\n"
         " cmp %0,r15          ; are we at the beginning of RAM?\n"
         " jne 1b              ; __low_level_init()\n"
         : : "i" (RAM_START), "i" (STACK_COLOR) : "r15"
         );
    }
  #endif /* COLOR_STACK */

  #if defined(NO_DATA_INIT)
    void __do_copy_data(void) __attribute__((section(".init4"),naked));
    void __do_copy_data() {
      __asm__ (
          "\n"
          //"nop; empty __do_copy_data() {} \n" // someplace to put a breakpoint
          );
    }
  #endif /* defined(NO_DATA_INIT) */

  #if defined(NO_BSS_INIT)
    void __do_clear_bss(void) __attribute__((section(".init4"),naked));
    void __do_clear_bss() {
      __asm__(
          "\n"
          //"nop; empty __do_clear_bss() {} \n" // someplace to put a breakpoint
          );
    }
  #endif /* defined(NO_BSS_INIT) */

} /* extern C */
#endif
