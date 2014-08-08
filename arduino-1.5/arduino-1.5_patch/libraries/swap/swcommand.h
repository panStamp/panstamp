/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
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
 * Creation date: 06/03/2013
 */

#ifndef _SWCOMMAND_H
#define _SWCOMMAND_H

#include "swpacket.h"


class SWCOMMAND : public SWPACKET
{
  public:
    /**
     * SWCOMMAND
     * 
     * Class constructor
     * 
     * @param destAddr Destination address
     * @param secNonce Security nonce
     * @param rAddr Register address
     * @param rId Register id
     * @param val New value
     * @param len Buffer length
     */
    SWCOMMAND(unsigned char destAddr, unsigned char secNonce, unsigned char rAddr, unsigned char rId, unsigned char *val, unsigned char len);
};
#endif
