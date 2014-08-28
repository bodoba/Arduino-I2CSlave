/*
 * ---------------------------------------------------------------------------------------
 * Shared definitions used by RPI master and Arduino Slave implementation
 * ---------------------------------------------------------------------------------------
 *
 * Copyright 2014 by Bodo Bauer <bb@bb-zone.com>
 *
 *
 * This file is part of I2C Arduino-Slave Library.
 *
 * The I2C Arduino Slave Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The I2C Arduino Slave Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 * --------------------------------------------------------------------------------------
 */

#ifndef I2CSlaveCommon_h
#define I2CSlaveCommon_h

/*
 * uncomment to enable verbose I2C Debug output
 */

//#define I2C_DEBUG

/* 
 * uncomment if you intend to use sonar sensors. 
 * Requires 'NewPing' library for Arduino
 */
#define USE_SONAR

/*
 * Command Tokens
 */
#define I2CCMD_SET_IO_PORT_MODE_INPUT    1
#define I2CCMD_SET_IO_PORT_MODE_OUTPUT   2
#define I2CCMD_SET_IO_PORT_MODE_SERVO    3
#define I2CCMD_SET_IO_PORT_HIGH          4
#define I2CCMD_SET_IO_PORT_LOW           5
#define I2CCMD_SET_SERVO_VALUE           6
#define I2CCMD_GET_IO_PORTS              7
#define I2CCMD_SET_SERVO_USEC            8
#define I2CCMD_GET_STATUS                9
#define I2CCMD_GET_ANALOG_READING       10

/*
 * IO Port Mode
 */
#define INPUT_PORT       1
#define OUTPUT_PORT      2
#define SERVO_PORT       3

/*
 * Address port by setting bits in 16bit value
 */
#define ARDUINO_IO_PORT_01   0x0001
#define ARDUINO_IO_PORT_02   0x0002
#define ARDUINO_IO_PORT_03   0x0004
#define ARDUINO_IO_PORT_04   0x0008
#define ARDUINO_IO_PORT_05   0x0010
#define ARDUINO_IO_PORT_06   0x0020
#define ARDUINO_IO_PORT_07   0x0040
#define ARDUINO_IO_PORT_08   0x0080
#define ARDUINO_IO_PORT_09   0x0100
#define ARDUINO_IO_PORT_10   0x0200
#define ARDUINO_IO_PORT_11   0x0400
#define ARDUINO_IO_PORT_12   0x0800
#define ARDUINO_IO_PORT_13   0x1000

#define ARDUINO_ANALOG_PORT_00   0
#define ARDUINO_ANALOG_PORT_01   1
#define ARDUINO_ANALOG_PORT_02   2
#define ARDUINO_ANALOG_PORT_03   3
#define ARDUINO_ANALOG_PORT_04   4
#define ARDUINO_ANALOG_PORT_05   5

/*
 * Adresses for four servos 
 */
#define ARDUINO_SERVO_A (unsigned char)0x01
#define ARDUINO_SERVO_B (unsigned char)0x02
#define ARDUINO_SERVO_C (unsigned char)0x04
#define ARDUINO_SERVO_D (unsigned char)0x08

#define ARDUINO_OK          10
#define ARDUINO_NOK         20
#define ARDUINO_BUFFER_SIZE 16

#endif