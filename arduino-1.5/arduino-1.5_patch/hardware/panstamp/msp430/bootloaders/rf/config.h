/**
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 17/09/2014
 */

#ifndef _CONFIG_H
#define _CONFIG_H

/**
 * Repeater options
 */
// Amount of transactions to be saved for evaluation before repeating a packet
// Maximum depth = 255
#define REPEATER_TABLE_DEPTH  20
// Expiration time (in ms) for any transaction
#define REPEATER_EXPIRATION_TIME  2000

/**
 * Addressing schema
 */
// Extended addresses (2 bytes)
//#define SWAP_EXTENDED_ADDRESS     1

#endif

