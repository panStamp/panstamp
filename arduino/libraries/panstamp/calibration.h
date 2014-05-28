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
 * Creation date: 11/13/2012
 */

#ifndef _CALIBRATION_H
#define _CALIBRATION_H

/**
 * Calibration thresholds
 */
#define TARGETCOUNT_MIN 62450   // Calibration target MIN
#define TARGETCOUNT_MAX 62550   // Calibration target MAX

/**
 * rcOscCalibrate
 * 
 * Calibrate internal RC oscillator
 *
 * Return:
 *   true  : calibration successfully done
 *   false : unable to complete calibration
 */
bool rcOscCalibrate(void);

#endif

