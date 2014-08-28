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

#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include <I2CSlaveCommon.h>
#ifdef USE_SONAR
#include <NewPing.h>
#endif

#ifndef I2CSlaveClass_h
#define I2CSlaveClass_h

class I2CSlaveClass {
public:
    I2CSlaveClass();
    
    void begin(int);
    void setStatusByte(unsigned char);
    
    // Callback functions for 'Wire' library
    static void receiveData (int);
    static void sendData    (void);
    
private:
    Servo ServoA;
    Servo ServoB;
    Servo ServoC;
    Servo ServoD;
    
    int           inputPorts;
    unsigned char outBuffer[ARDUINO_BUFFER_SIZE];
    int           outBytes;
    unsigned char status;
    
#ifdef USE_SONAR
    NewPing *sonar;
    void setSonarPort (int, int);
    void getSonarReading (unsigned char *);
#endif
    
    void getAnalogReading (unsigned char, unsigned char *);
    void setIOPortMode (int, int);
    void setIOPortModeServo (int, unsigned char);
    void setIOPort (int, int);
    void getIOPorts (unsigned char *);
    void setServoValue (int, unsigned char);
    void setServoUSec (int, unsigned char);
};

extern I2CSlaveClass I2CSlave;
#endif