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
 * Creation date: 07/23/2014
 */

#ifndef STORAGE_h
#define STORAGE_h

#include "cc430info.h"

#define DEFAULT_NVOLAT_SECTION  INFOMEM_SECTION_D

#define STORAGE  CC430INFO

/**
 * RAM memory
 */
const uint16_t RAM_END_ADDRESS = 0x2BFF;

/**
 * Flash memory
 */
const uint16_t WIRELESS_BOOT_ADDR = 0x8000;

#endif

