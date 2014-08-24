#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "ArduinoSlave/I2CSlaveCommon.h"
#include "ArduinoSlave/ArduinoSlave.h"

#define I2C_ARDUINO 0x04
#define I2C_DEVICE  "/dev/i2c-1"

#define LED_RIGHT    ARDUINO_IO_PORT_08
#define LED_LEFT     ARDUINO_IO_PORT_09

#define BUTTON       ARDUINO_IO_PORT_07


int main(int argc, char **argv)
{
    ArduinoSlave arduino( I2C_ARDUINO, I2C_DEVICE );

    // LED Ports
    if ( !arduino.setIoPortModeOutput(LED_RIGHT | LED_LEFT)) {
        printf ("[FATAL] Could not set up LED ports\n");
        exit(1);
    }
    arduino.setIoPortLow(LED_RIGHT);
    arduino.setIoPortLow(LED_LEFT);
    usleep(5000);
    
    // Input ports
    if ( !arduino.setIoPortModeInput(BUTTON)) {
        printf ("[FATAL] Could not set up BUTTON port\n");
        exit(1);
    }

    usleep(5000);
    bool leftLED       = FALSE;
    bool rightLED      = FALSE;
    bool buttonPressed = FALSE;
    time_t now, past=(time_t)0;
    int inputPorts;
    
    while ( TRUE ) {
        // blink left LED once a second
        now=time(NULL);
        if ( now != past ) {
            if ( leftLED ) {
                arduino.setIoPortHigh(LED_LEFT);
            } else {
                arduino.setIoPortLow(LED_LEFT);
            }
            leftLED = !leftLED;
            past = now;
        }
        
        // Toggle right LED on button release
        inputPorts=arduino.getIoPorts();
        if ( buttonPressed ) {
            if ( inputPorts & BUTTON ) {
                if ( rightLED ) {
                    arduino.setIoPortHigh(LED_RIGHT);
                } else {
                    arduino.setIoPortLow(LED_RIGHT);
                }
                rightLED = !rightLED;
                buttonPressed = FALSE;
            }
        } else if ( !(inputPorts & BUTTON) ) {
            buttonPressed = TRUE;
        }
        
        // give ARDUINO some rest
        usleep(10000);
    }
    return 0;
}
