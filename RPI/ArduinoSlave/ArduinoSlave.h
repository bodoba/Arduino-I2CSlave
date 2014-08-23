/*
 * ---------------------------------------------------------------------------------------
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


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef ARDUINO_SLAVE_H
#define ARDUINO_SLAVE_H

#ifndef TRUE
#define TRUE	(1==1)
#define FALSE	(0==1)
#endif

#define ARDUINO_DELAY 1000 // wait time for usleep after write commands
#define ARDUINO_HEALING_FACTOR 100

class ArduinoSlave {
public:
    ArduinoSlave( uint8_t ardAddr, const char *i2cdevice );
    bool setIoPortModeInput ( int ports );
    bool setIoPortModeOutput ( int ports );
    bool setIoPortHigh ( int ports );
    bool setIoPortLow ( int ports );
    uint16_t getIoPorts ( void );
    bool setIoPortModeServo ( unsigned char servo, int port );
    bool setServoValue ( unsigned char servo, int value );
    bool setServoUSec ( unsigned char servo, int value );
    bool getStatus( uint8_t *status );
#ifdef USE_SONAR
    bool setSonarPort( uint16_t port, uint16_t maxDistance );
    uint16_t getSonarReading( void );
#endif
    
private:
    uint8_t arduinoAddress;
    const char *i2cDevice;
    char i2cErrorString[128];
    bool i2cWriteCmd( uint8_t command, uint8_t length, uint8_t *data );
    bool i2cReadBytes( uint8_t length, uint8_t *data );
    bool checkResponse ( const char* name, bool success );
    int numReadErrors;
    int numWriteErrors;
    int numReadSuccess;
    int numWriteSuccess;
};
#endif
