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
#include <errno.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "I2CSlaveCommon.h"
#include "ArduinoSlave.h"

ArduinoSlave::ArduinoSlave( uint8_t ardAddr, const char *i2cDev ) {
    arduinoAddress  = ardAddr;
    i2cDevice       = i2cDev;
}

bool ArduinoSlave::checkResponse( const char* name, bool success ) {
    if ( !success ) {
        fprintf(stderr, "[ERROR] %s failed. %s\n", name, i2cErrorString);
    } else {
        uint8_t status;
        success = i2cReadBytes(1, &status);
        if ( !success ) {
            fprintf(stderr, "[ERROR] %s read response failed. %s\n", name, i2cErrorString);
        } else {
            if ( status != ARDUINO_OK) {
                fprintf(stderr, "[ERROR] %s did not receive OK from slave device (got %d).\n", name, status);
                success = FALSE;
            }
        }
    }
    return (success);
}

bool ArduinoSlave::setIoPortModeInput ( int ports ) {
    uint8_t data[2];
    bool    success;
    
    data[0] = (unsigned char)  (ports & 0x00ff);
    data[1] = (unsigned char) ((ports & 0xff00) >> 8 );
    success = i2cWriteCmd(I2CCMD_SET_IO_PORT_MODE_INPUT, 2, data);
    return (checkResponse ("ArduinoSlave::setIoPortModeInput", success ));
}

bool ArduinoSlave::setIoPortModeOutput ( int ports ) {
    uint8_t data[2];
    bool    success;
    
    data[0] = (unsigned char)  (ports & 0x00ff);
    data[1] = (unsigned char) ((ports & 0xff00) >> 8 );
    success = i2cWriteCmd(I2CCMD_SET_IO_PORT_MODE_OUTPUT, 2, data);
    return (checkResponse ("ArduinoSlave::setIoPortModeOutput", success ));
}

bool ArduinoSlave::setIoPortHigh ( int ports ) {
    uint8_t data[2];
    bool    success;
    
    data[0] = (unsigned char)  (ports & 0x00ff);
    data[1] = (unsigned char) ((ports & 0xff00) >> 8 );
    success = i2cWriteCmd(I2CCMD_SET_IO_PORT_HIGH, 2, data);
    return (checkResponse ("ArduinoSlave::setIoPortHigh", success ));
}

bool ArduinoSlave::setIoPortLow ( int ports ) {
    uint8_t data[2];
    bool    success;
    
    data[0] = (unsigned char)  (ports & 0x00ff);
    data[1] = (unsigned char) ((ports & 0xff00) >> 8 );
    success = i2cWriteCmd(I2CCMD_SET_IO_PORT_LOW, 2, data);
    return (checkResponse ("ArduinoSlave::setIoPortLow", success ));
}

bool ArduinoSlave::setIoPortModeServo ( unsigned char servo, int port ) {
    uint8_t data[3];
    bool    success;
    
    data[0] = (unsigned char)  (port & 0x00ff);
    data[1] = (unsigned char) ((port & 0xff00) >> 8 );
    data[2] = servo;
    success = i2cWriteCmd(I2CCMD_SET_IO_PORT_MODE_SERVO, 3, data);
    return (checkResponse ("ArduinoSlave::setIoPortModeServo", success ));
}

bool ArduinoSlave::setServoValue ( unsigned char servo, int value ) {
    uint8_t data[4];
    bool    success;
    
    data[0] = (unsigned char)  (value & 0x00ff);
    data[1] = (unsigned char) ((value & 0xff00) >> 8 );
    data[2] = servo;
    success = i2cWriteCmd(I2CCMD_SET_SERVO_VALUE, 3, data);
    return (checkResponse ("ArduinoSlave::setIoPortServoValue", success ));
}

bool ArduinoSlave::setServoUSec ( unsigned char servo, int value ) {
    uint8_t data[4];
    bool    success;
    
    data[0] = (unsigned char)  (value & 0x00ff);
    data[1] = (unsigned char) ((value & 0xff00) >> 8 );
    data[2] = servo;
    success = i2cWriteCmd(I2CCMD_SET_SERVO_USEC, 3, data);
    return (checkResponse ("ArduinoSlave::setIoPortServoUSec", success ));
}

bool ArduinoSlave::getStatus (uint8_t *status) {
    bool success = FALSE;
    
    if ( i2cWriteCmd(I2CCMD_GET_STATUS, 0, NULL) ) {
        success = i2cReadBytes(1, status);
    }
    if ( !success ) {
        fprintf(stderr, "[ERROR] ArduinoSlave::getStatus(...) failed. %s\n", i2cErrorString);
    }
    return (success);
}

uint16_t ArduinoSlave::getIoPorts ( void ) {
    bool     success = FALSE;
    uint8_t  data[2] = {0,0};
    uint16_t ioPorts;

    if ( i2cWriteCmd(I2CCMD_GET_IO_PORTS, 0, NULL) ) {
        success = i2cReadBytes(2, data);
    }
    if ( !success ) {
        fprintf(stderr, "[ERROR] ArduinoSlave::getIoPorts(...) failed. %s\n", i2cErrorString);
    }
    ioPorts = ( data[0] & 0x00ff ) | ((data[1] << 8) & 0xff00 );
    return (ioPorts);
}


#ifdef USE_SONAR
bool ArduinoSlave::setSonarPort( uint16_t port, uint16_t maxDistance ) {
    uint8_t data[5];
    bool    success;
    
    data[0] = (unsigned char)  (port & 0x00ff);
    data[1] = (unsigned char) ((port & 0xff00) >> 8 );
    data[2] = (unsigned char)  (maxDistance & 0x00ff);
    data[3] = (unsigned char) ((maxDistance & 0xff00) >> 8 );
    success = i2cWriteCmd(I2CCMD_SET_SONAR_PORT, 4, data);
    usleep(ARDUINO_DELAY);
    return (checkResponse ("ArduinoSlave::setSonarPort", success ));
}

uint16_t ArduinoSlave::getSonarReading( void ) {
    bool     success = FALSE;
    uint8_t  data[2] = {0,0};
    uint16_t sonarReading;
    
    if ( i2cWriteCmd(I2CCMD_GET_SONAR_READING, 0, NULL) ) {
        success = i2cReadBytes(2, data);
    }
    if ( !success ) {
        fprintf(stderr, "[ERROR] ArduinoSlave::getSonarReading(...) failed. %s\n", i2cErrorString);
    }
    sonarReading = ( data[0] & 0x00ff ) | ((data[1] << 8) & 0xff00 );
    return (sonarReading);
}
#endif

bool ArduinoSlave::i2cWriteCmd( uint8_t command, uint8_t length, uint8_t *data ) {
    int8_t buf[ARDUINO_BUFFER_SIZE];
    int8_t count=0;
    int    fd;
    bool   success = FALSE;
    
    i2cErrorString[0] = (char)0;
    
    if (length >= ARDUINO_BUFFER_SIZE) {
        sprintf(i2cErrorString, "Byte write count (%d) > %d", ARDUINO_BUFFER_SIZE, length);
    } else {
        fd = open(i2cDevice, O_RDWR);
        if (fd < 0) {
            sprintf(i2cErrorString, "Failed to open device %s: %s", i2cDevice, strerror(errno));
        } else {
            if (ioctl(fd, I2C_SLAVE, arduinoAddress) < 0) {
                sprintf(i2cErrorString, "Failed to select device %s: %s", i2cDevice, strerror(errno));
            } else {
                buf[0] = command;
                if ( length ) {
                    memcpy(buf+1,data,length);
                }
                count = write(fd, buf, length+1);
                if (count < 0) {
                    sprintf(i2cErrorString, "Failed to write device %s (%d): %s", i2cDevice, count, ::strerror(errno));
                } else {
                    if (count != length+1) {
                        sprintf(i2cErrorString, "Short write to device %s, expected %d, got %d", i2cDevice, length+1, count);
                    } else {
                        success = TRUE;
                    }
                }
            }
            close(fd);
        }
    }
    usleep(ARDUINO_DELAY);
    return(success);
}

bool ArduinoSlave::i2cReadBytes ( uint8_t length, uint8_t *data ) {
    bool success = FALSE;
    i2cErrorString[0] = (char)0;
    int8_t count = 0;
    int fd = open(i2cDevice, O_RDWR);
    
    if (fd < 0) {
        sprintf(i2cErrorString, "Failed to open device %s: %s", i2cDevice, strerror(errno));
    } else {
        if (ioctl(fd, I2C_SLAVE, arduinoAddress) < 0) {
            sprintf(i2cErrorString, "Failed to select device %s: %s", i2cDevice, strerror(errno));
        } else {
            count = read(fd, data, length);
            if (count < 0) {
                sprintf(i2cErrorString, "Failed to read device %s(%d): %s", i2cDevice, count, ::strerror(errno));
            } else {
                if (count != length) {
                    sprintf(i2cErrorString, "Short read  from device %s, expected %d, got %d", i2cDevice, length, count);
                } else {
                    success = TRUE;
                }
            }
        }
        close(fd);
    }
    return(success);
}


