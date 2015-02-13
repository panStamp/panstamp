/*
 * logic.h - metatemplate  logic routines
 *
 * Created: Nov-12-2012
 *  Author: rick@kimballsoftware.com
 *    Date: 03-27-2013
 * Version: 1.0.1
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
 */

#ifndef LOGIC_H_
#define LOGIC_H_

/**
 * struct_is_power_of_two - power of 2 checker
 *   enforce at compile time that SIZE is power of 2 and >= 2
 */

template<unsigned SIZE>
struct is_power_of_two {
  enum {val = (SIZE >= 2) & (SIZE > 1) & !(SIZE & (SIZE - 1))};
  static const unsigned badSIZE[(val == 1) ? 1 : -1]; // SIZE is not a power of 2 if you error here.
};

enum yesno_e { NO=0, YES=1 };

#endif /* LOGIC_H_ */
