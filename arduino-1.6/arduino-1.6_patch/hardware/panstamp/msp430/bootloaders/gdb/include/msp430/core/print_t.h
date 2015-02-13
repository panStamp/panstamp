/*
 *  print.h - all dancing, all singing print_t class template
 *
 *  CRTP (Curiously Recurring Template Pattern ) style print class template
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-16-2013
 * Version: 1.0.2
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

#ifndef PRINT_T_H_
#define PRINT_T_H_

enum _endl_enum { endl };

/*
 * print_t - full function print class template
 *
 * Desc: The print_t template provide a common binary to ASCII print implementation.
 *       Think of it as itoa() with templates.  The user is expected to provide
 *       a Write class that exposes a function called write_impl().
 */

template<typename Writer, typename u16_print_t=unsigned, typename u32_print_t=uint32_t>
class print_t {

private:

  //--------------------------------------------------------------------------------
  // these _functions() are private internal functions not fit for human consumption

  /*
   * _print_base() - binary to ASCII conversion using arbitrary base
   *
   * _print_base() is implemented as a template to allow for the
   * smallest possible code. User code that only prints 16 bit values
   * will produce the smallest code with the default settings of
   * u16_print_t=uint16_t and u32_print_t=uint32_t.  Code that prints a
   * mix of 16 bit and 32 bit values will produce smaller code if both
   * the 16 bit and 32 bit versions use the a single _print_base()
   * implementation . In that case, set u16_print_t=uint32_t and
   * u32_print_t=uint32_t. When used in this fashion, all conversions
   * are done by one function; _print_base<uint32_t>() so that all
   * print results are completed using 32 bit integers. This results
   * in smaller final code as there will only be one set of
   * _udiv[xx]3/_umod[xx]3 functions included from libc.a
   *
   * Note: Other implementations of binary to base X conversion routines
   *       have gone to great lengths to avoid using division. However,
   *       in this case it seems this is a reasonable approach with
   *       using msp430-gcc's effcient implementation of _udiv3 and _umod3.
   *
   * Con:  we use the stack to assemble the print string, it might use
   *       up to 34+ bytes of stack.
   *
   * Alternate approaches tried:
   *
   * o itoa() - 126 bytes larger, 2x stack usage, doesn't deal with 16/32 bit
   *
   * o looping with no division, ok for decimal but not for all base routine
   *   coverage, tables takes up more room than division/modulo. Also, this
   *   method can't take advantage of compiler's power of 2 bases optimizations
   *   that use 'and' and 'shift' operators to implement div and modulo.
   *
   * o div_t - 64 bytes larger, doesn't optimize for 16/32 bit
   *
   * Typical conversion times:
   *    ~60us for 3 digits, ~350us for 5 digits @ 16MHz
   *
   */

  template<typename T> /* T is usually uint16_t or uint32_t */
  void _print_base(T n, const base_e base) {
    char buf[(8 * sizeof(T)) + 1]; // (8 bits * N bytes) + 1 for NULL
    char *str = &buf[sizeof(buf) - 1];
#if 0
    P1_0::high(); /* toggle on for debug timing */
#endif

    *str = '\0'; // work from least significant digit to most
    do {
      unsigned digit = n % base;
     *--str = digit < 10 ? digit + '0' : digit + 'A' - 10;
    } while( (n /= base) );

#if 0
    P1_0::low(); /* toggle off for debug timing */
#endif

    _puts(str);
  }

  void _puts(const unsigned char *s) {
    while (*s) {
      write(*s++);
    }
  }

  void _puts(const char *s) {
    _puts(reinterpret_cast<const unsigned char *>(s));
  }


  //------------------------------------------------------------
  // public
  //------------------------------------------------------------
public:

  void print(const char * s) {
    _puts(s);
  }

  void print(char *s) {
    _puts(s);
  }

  //----------------- signed --------------------------
  //

  void print(int8_t i, const base_e base=DEC) {
    if ( base == DEC && i < 0 ) {
      write((uint8_t)'-');
      i = -i;
    }
    print((uint8_t)i, base);
  }

  void print(int16_t i, const base_e base=DEC) {
    if ( base == DEC && i < 0 ) {
      write((uint8_t)'-');
      i = -i;
    }
    print((uint16_t)i, base);
  }

  void print(int32_t i, const base_e base=DEC) {
    if ( base == DEC && i < 0 ) {
      write((uint8_t)'-');
      i = -i;
    }
    print((uint32_t)i, base);
  }

  //----------------- unsigned --------------------------
  //

  void print(uint8_t u, const base_e base=DEC) {
    if ( base )
      _print_base<u16_print_t>(u, base);
    else {
      write((uint8_t)u);
    }
  }

  void print(uint16_t u, const base_e base=DEC) {
    if ( base )
      _print_base<u16_print_t>(u, base);
    else {
      write((uint8_t)u);
    }
  }

  void print(uint32_t u, const base_e base=DEC) {
    if (base) {
      _print_base<u32_print_t>(u, base);
    }
    else {
      write((uint8_t)u);
    }
  }

  //----------------- specials --------------------------
  //

  void print(_endl_enum) {
    println();
  }

  void println(void) {
    write((uint8_t)'\r');
    write((uint8_t)'\n');
  }

  /*
   * print() - fix16_t fixed point values
   */
  void print(const fix16_t value, const unsigned digits) {
    _print_fix16(value, digits > 5 ? 5 : digits);
  }

  /*
   * print(float) - probably should be avoided if you care about small size code
   *
   */
  void print(float f, unsigned decimal_places) {
    return _print_float(f, decimal_places);
  }

  /*
   * printf() - you probably don't want to use this either if you care about code size
   *
   * minimal printf() implementation based on implementation from Kevin Timmermans's
   * tiny printf see: http://forum.43oh.com/topic/1289-tiny-printf-c-version/
   */

  void printf(const char *format, ...) {
    long n;
    int i;
    char c;
    va_list a;

    va_start(a, format);

    while ((c = *format++)) {
      if (c == '%') {
        switch (c = *format++) {
          case 's': // String
            print(va_arg(a, char*));
            break;

          case 'c': // Char
            write((uint8_t)va_arg(a, int));
            break;

          case 'i': // 16 bit Integer
          case 'u': // 16 bit Unsigned
            i = va_arg(a, int);
            if(c == 'i' && i < 0) i = -i, write('-');
            print((unsigned)i);
            break;

          case 'l': // 32 bit Long
          case 'n': // 32 bit uNsigned loNg
            n = va_arg(a, long);
            if(c == 'l' && n < 0) n = -n, write('-');
            print((unsigned long)n);
            break;

          case 'x': // 16 bit heXadecimal
            i = va_arg(a, unsigned);
            print((uint16_t)i,HEX);
            break;

          case 0:
            return;

  #if 0 // you could enable these if you have lots of flash space
          case 'b':
            print((uint16_t)va_arg(a, uint16_t),BIN);
            break;

          case 'o': // 16 bit Octal
            print((uint16_t)va_arg(a, uint16_t),OCT);
            break;

          case 'O': // 32 bit Octal
            print((uint32_t)va_arg(a, uint32_t),OCT);
            break;

          case 'X':// 32 bit heXadecimal
            n = va_arg(a, long);
            print((unsigned long)n,HEX);
            break;
  #endif

          default:
            goto bad_fmt;
        }
      }
      else {
   bad_fmt:
        write(c);
      }
    }
    va_end(a);
  }

  /*
   * write - write 1 byte character
   *
   * Note: Writer class expects us to implement this function
   */

  int write(uint8_t c) {
    return static_cast<Writer*>(this)->write_impl(c);
  }

  /* TODO: explain why I avoided println(n) functions to discourage usage, as they causes bloat */

private:

  /*
   * _print_fix16() - similar to fix16_to_str routines
   *
   *  optimized to use write routines so less stack is used
   */
  void _print_fix16(const fix16_t value, const unsigned digits) {
    /* 5 decimals is enough for full fix16_t precision */
    static const uint32_t scales[8] = { 1, 10, 100, 1000, 10000, 100000, 100000, 100000 };

    uint32_t uvalue = (value >= 0) ? value : -value;
    /* print integer results */
    if (value < 0) {
      write((uint8_t) '-');
    }

    /* Separate the integer and decimal parts of the value */
    unsigned int_part = uvalue >> 16;
    uint32_t remainder = uvalue & 0xFFFF;
    uint32_t scale = scales[digits & 7];
    remainder = fix16_mul(remainder, scale);

    if (remainder >= scale) {
      /* Handle carry from decimal part */
      int_part++;
      remainder -= scale;
    }

    print(int_part);

    /* print decimal results with zero fill */
    if (digits > 0) {
      write((uint8_t)'.');
      bool skip = false;

      while (scale/=10) {
        unsigned digit = (remainder / scale);

        if (!skip || digit || scale == 1) {
          skip = false;
          write((uint8_t)('0' + digit));
          remainder %= scale;
        }
      }
    }
  }

  /*
   * _printFloat() - .. hmmm do you really want to use this?
   *
   * hint: .. probably not!
   */

  void _print_float(float number, unsigned digits) {
    // Handle negative numbers
     if (number < 0.0f)
     {
        write('-');
        number = -number;
     }

     // Round correctly so that print(1.999, 2) prints as "2.00"
     float rounding = 0.5f;
     for (unsigned i=0; i < digits; i++) {
       rounding /= 10.0f;
     }

     number += rounding;

     // Extract the integer part of the number and print it
     unsigned long int_part = (unsigned long)number;
     float remainder = number - (float)int_part;
     print(int_part);

     // Print the decimal point, but only if there are digits beyond
     if (digits > 0) {
       write('.');
     }

     // Extract digits from the remainder one at a time
     while (digits-- > 0) {

       remainder *= 10.0f;
       unsigned digit = unsigned(remainder);
       print(digit);
       remainder -= digit;
     }
   }

};

/*
 * print_t insertion operator support
 *
 * similar and mostly based on ideas in:
 *  http://arduiniana.org/libraries/streaming/
 */

// Serial << 1234; // print ascii decimal version of 1234

template<typename PRINT_T, typename T>
inline PRINT_T &operator << (PRINT_T &obj, const T arg) {
  obj.print(arg); return obj;
}

//   Serial << _HEX(1234); // print ascii hex version of decimal 1234

template<typename T>
struct _BASE {
  const T n;
  const base_e base;

  _BASE(const T _n, const base_e _base): n(_n), base(_base) {}
};

#define _RAW(n) _BASE<typeof((n))>(n,RAW)
#define _BIN(n) _BASE<typeof((n))>(n,BIN)
#define _OCT(n) _BASE<typeof((n))>(n,OCT)
#define _DEC(n) _BASE<typeof((n))>(n,DEC)
#define _HEX(n) _BASE<typeof((n))>(n,HEX)

template<typename PRINT_T, typename T>
inline PRINT_T &operator << (PRINT_T &obj, const _BASE<T> &arg) {
  obj.print(arg.n, arg.base); return obj;
}

//   Serial << _FLOAT(gps_latitude, 6); // 6 digits of precision

template<typename T>
struct _FBASE
{
  const T val;
  const unsigned digits;

  _FBASE(const T v, const unsigned d): val(v), digits(d) {}
};

#define _FLOAT(f,d) _FBASE<float>((f),(d))

template<typename PRINT_T, typename T>
inline PRINT_T &operator <<(PRINT_T &obj, const _FBASE<T> &arg) {
  obj.print(arg.val, arg.digits); return obj;
}

//   Serial << _FIX16(temp_celsius, 2); // 2 digits of precision

template<typename T>
struct _FIX16BASE
{
  const T val;
  const unsigned digits;

  _FIX16BASE(const T v, const unsigned d): val(v), digits(d) {}
};

#define _FIX16(f16,d) _FIX16BASE<fix16_t>((f16),(d))

template<typename PRINT_T, typename T>
inline PRINT_T &operator <<(PRINT_T &obj, const _FIX16BASE<T> &arg) {
  obj.print(arg.val, arg.digits); return obj;
}

#endif /* PRINT_T_H_ */
