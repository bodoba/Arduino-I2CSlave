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

#include <I2CSlaveCommon.h>
#include <I2CSlave.h>

#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

#ifdef I2C_DEBUG
#define DEBUG_PRINT(a)  Serial.print(a)
#define DEBUG_PRINTLN(a) Serial.println(a)
#else
#define DEBUG_PRINT(a)
#define DEBUG_PRINTLN(a)
#endif

/*
 * --------------------------------------------------------------------------------------
 *  P U B L I C   I N T E R F A C E
 * --------------------------------------------------------------------------------------
 */

// There is alwazs an I2CSlave object. If you don"t need it, dont link this library
I2CSlaveClass I2CSlave = I2CSlaveClass();
I2CSlaveClass::I2CSlaveClass ()
{
    inputPorts = 0;
}

/** Attach device to I2C bus
 * @param Address to be used for this device
 */
void I2CSlaveClass::begin ( int slaveAddress )
{
    // Reset internal state
    status     = 0;
    inputPorts = 0;
    outBytes   = 0;
    
    // Register device as i2c slave
    Wire.begin(slaveAddress);
    
    // define callbacks for i2c communication
    Wire.onReceive(receiveData);
    Wire.onRequest(sendData);
}

/** Provide status byte to be read by master
 * @param Status
 */
void I2CSlaveClass::setStatusByte ( unsigned char status ) {
    I2CSlave.status = status;
}

/** Receive data callback
 * Internal function. Needs to be public for use by Wire library
 * @param number of bytes to read from bus
 */
void I2CSlaveClass::receiveData( int count )
{
    unsigned char inBuffer[ARDUINO_BUFFER_SIZE];
    int           inBytes=0;
    
    for ( inBytes=0; inBytes<ARDUINO_BUFFER_SIZE && Wire.available(); inBytes++ ) {
        inBuffer[inBytes] = Wire.read();
    }
    
    // Byte 0 holds the command token
    switch ( inBuffer[0] ) {
            
        case I2CCMD_SET_IO_PORT_MODE_INPUT:
            I2CSlave.setIOPortMode ( INPUT,  word(inBuffer[2],inBuffer[1]) );
            I2CSlave.outBuffer[0]=ARDUINO_OK;
            I2CSlave.outBytes=1;
            break;
            
        case I2CCMD_SET_IO_PORT_MODE_OUTPUT:
            I2CSlave.setIOPortMode ( OUTPUT, word(inBuffer[2],inBuffer[1]) );
            I2CSlave.outBuffer[0]=ARDUINO_OK;
            I2CSlave.outBytes=1;
            break;
            
        case I2CCMD_SET_IO_PORT_HIGH:
            I2CSlave.setIOPort ( HIGH,  word(inBuffer[2],inBuffer[1]) );
            I2CSlave.outBuffer[0]=ARDUINO_OK;
            I2CSlave.outBytes=1;
            break;
            
        case I2CCMD_SET_IO_PORT_LOW:
            I2CSlave.setIOPort ( LOW, word(inBuffer[2],inBuffer[1]) );
            I2CSlave.outBuffer[0]=ARDUINO_OK;
            I2CSlave.outBytes=1;
            break;
            
        case I2CCMD_SET_IO_PORT_MODE_SERVO:
            I2CSlave.setIOPortModeServo ( word(inBuffer[2],inBuffer[1]), inBuffer[3] );
            I2CSlave.outBuffer[0]=ARDUINO_OK;
            I2CSlave.outBytes=1;
            break;
            
        case I2CCMD_SET_SERVO_VALUE:
            I2CSlave.setServoValue ( word(inBuffer[2],inBuffer[1]), inBuffer[3] );
            I2CSlave.outBuffer[0]=ARDUINO_OK;
            I2CSlave.outBytes=1;
            break;
            
        case I2CCMD_SET_SERVO_USEC:
            I2CSlave.setServoUSec ( word(inBuffer[2],inBuffer[1]), inBuffer[3] );
            I2CSlave.outBuffer[0]=ARDUINO_OK;
            I2CSlave.outBytes=1;
            break;
            
        case I2CCMD_GET_IO_PORTS:
            I2CSlave.getIOPorts( I2CSlave.outBuffer );
            DEBUG_PRINT( "Port Data 0:" ); DEBUG_PRINT(   I2CSlave.outBuffer[0] );
            DEBUG_PRINT( " 1:" );          DEBUG_PRINTLN( I2CSlave.outBuffer[1] );
            I2CSlave.outBytes=2;
            break;
            
        case I2CCMD_GET_STATUS:
            DEBUG_PRINTLN( "Get STATUS" );
            I2CSlave.outBuffer[0]=I2CSlave.status;
            I2CSlave.outBytes=1;
            break;
        
        case I2CCMD_GET_ANALOG_READING:
            I2CSlave.getAnalogReading(inBuffer[1], I2CSlave.outBuffer);
            I2CSlave.outBytes=1;
            break;
            
        default:
            DEBUG_PRINT( "[ERROR] Unhandled Command Token" );
            DEBUG_PRINTLN( (int)inBuffer[0] );
            break;
    }
}

/** Send data callback
 * Internal function. Needs to be public for use by Wire library
 */
void I2CSlaveClass::sendData(void)
{
    if ( I2CSlave.outBytes ) {
        Wire.write(I2CSlave.outBuffer, I2CSlave.outBytes);
        DEBUG_PRINT( "Sent " );
        DEBUG_PRINT( I2CSlave.outBytes );
        DEBUG_PRINTLN( " Bytes : " );
        I2CSlave.outBytes=0;
    } else {
        DEBUG_PRINT( "[ERROR] Unhandled Send Data Request" );
    }
}

/*
 * --------------------------------------------------------------------------------------
 *  P R I V A T E    I N T E R N A L    I N T E R F A C E
 * --------------------------------------------------------------------------------------
 */

/** Read analog port
 */
void I2CSlaveClass::getAnalogReading (unsigned char pin, unsigned char *value) {
    *value = analogRead(pin);
}

/** Set port mode to input or output
 */
void I2CSlaveClass::setIOPortMode ( int mode, int mask )
{
    int bit=0;
    for ( int bit = 0 ; bit < 13; bit++ ) {
        if ( mask & ( 1 << bit ) ) {
            pinMode ( bit+1, mode );
            
#ifdef I2C_DEBUG
            DEBUG_PRINT( "Port " );
            DEBUG_PRINT( bit+1 );
            if ( mode == INPUT ) {
                DEBUG_PRINTLN ( " INPUT" );
            } else {
                DEBUG_PRINTLN ( " OUTPUT" );
            }
#endif
        }
    }
    if ( mode == INPUT ) {
        inputPorts = mask;
    }
}

/** Read Input Ports
 */
void I2CSlaveClass::getIOPorts ( unsigned char *data )
{
    int portStatus = 0;
    int level      = 0;
    int bit        = 0;
    
    for ( int bit = 0 ; bit < 13; bit++ ) {
        if ( inputPorts & ( 1 << bit ) ) {
            level = digitalRead ( bit+1 );
            DEBUG_PRINT( "Port " );
            DEBUG_PRINT( bit+1 );
            if ( level == HIGH ) {
                DEBUG_PRINTLN( " HIGH" );
                portStatus |= ( 1 << bit );
            } else {
                DEBUG_PRINTLN( " LOW" );
            }
        }
    }
    data[0] = (unsigned char)  (portStatus & 0x00ff);
    data[1] = (unsigned char) ((portStatus & 0xff00) >> 8 );
}


void I2CSlaveClass::setIOPortModeServo ( int port, unsigned char servo ) {
    int bit=0;
    for ( int bit = 0 ; bit < 13; bit++ ) {
        if ( port & ( 1 << bit ) ) {
            DEBUG_PRINT( "Port " );
            DEBUG_PRINT( bit+1 );
            DEBUG_PRINT( " Servo " );
            switch ( servo ) {
                case ARDUINO_SERVO_A:
                    ServoA.attach(bit+1);
                    DEBUG_PRINTLN(" A");
                    break;
                case ARDUINO_SERVO_B:
                    ServoB.attach(bit+1);
                    DEBUG_PRINTLN(" B");
                    break;
                case ARDUINO_SERVO_C:
                    ServoC.attach(bit+1);
                    DEBUG_PRINTLN(" C");
                    break;
                case ARDUINO_SERVO_D:
                    ServoD.attach(bit+1);
                    DEBUG_PRINTLN(" D");
                    break;
            }
            break;
        }
    }
}

/** Set output ports to HIGH or LOW
 */
void I2CSlaveClass::setIOPort ( int level, int mask )
{
    int bit=0;
    for ( int bit = 0 ; bit < 13; bit++ ) {
        if ( mask & ( 1 << bit ) ) {
            digitalWrite ( bit+1, level );
#ifdef I2C_DEBUG
            Serial.print ( "Port " );
            Serial.print ( bit+1 );
            if ( level == HIGH ) {
                Serial.println ( " HIGH" );
            } else {
                Serial.println ( " LOW" );
            }
#endif
        }
    }
}

void I2CSlaveClass::setServoValue ( int value, unsigned char servo ) {
    DEBUG_PRINT( "Servo" );
    switch ( servo ) {
        case ARDUINO_SERVO_A:
            ServoA.write(value);
            DEBUG_PRINT(" A: ");
            break;
        case ARDUINO_SERVO_B:
            ServoB.write(value);
            DEBUG_PRINT(" B: ");
            break;
        case ARDUINO_SERVO_C:
            ServoC.write(value);
            DEBUG_PRINT(" C: ");
            break;
        case ARDUINO_SERVO_D:
            ServoD.write(value);
            DEBUG_PRINT(" D: ");
            break;
    }
    DEBUG_PRINTLN(value);
}

void I2CSlaveClass::setServoUSec ( int value, unsigned char servo ) {
    DEBUG_PRINT( "Servo" );
    switch ( servo ) {
        case ARDUINO_SERVO_A:
            ServoA.writeMicroseconds(value);
            DEBUG_PRINT(" A: ");
            break;
        case ARDUINO_SERVO_B:
            ServoB.writeMicroseconds(value);
            DEBUG_PRINT(" B: ");
            break;
        case ARDUINO_SERVO_C:
            ServoC.writeMicroseconds(value);
            DEBUG_PRINT(" C: ");
            break;
        case ARDUINO_SERVO_D:
            ServoD.writeMicroseconds(value);
            DEBUG_PRINT(" D: ");
            break;
    }
    DEBUG_PRINTLN(value);
}
