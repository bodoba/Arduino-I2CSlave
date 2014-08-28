#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#include "ArduinoSlave/I2CSlaveCommon.h"
#include "ArduinoSlave/ArduinoSlave.h"

#define I2C_ARDUINO 0x04
#define I2C_DEVICE  "/dev/i2c-1"

#define LED_RED      ARDUINO_IO_PORT_08
#define LED_GREEN    ARDUINO_IO_PORT_09

#define BUTTON       ARDUINO_IO_PORT_07

int main(int argc, char **argv)
{
    ArduinoSlave arduino( I2C_ARDUINO, I2C_DEVICE );

    // LED Ports
    if ( !arduino.setIoPortModeOutput(LED_RED | LED_GREEN)) {
        printf ("[FATAL] Could not set up LED ports\n");
        exit(1);
    }
    arduino.setIoPortLow(LED_RED);
    arduino.setIoPortLow(LED_GREEN);
    usleep(5000);
    
    // Input ports
    if ( !arduino.setIoPortModeInput(BUTTON)) {
        printf ("[FATAL] Could not set up BUTTON port\n");
        exit(1);
    }
    usleep(5000);
    
    bool ledGreen      = FALSE;
    bool ledRed        = FALSE;
    bool buttonPressed = FALSE;
    time_t now, past=(time_t)0;
    int inputPorts;
    
    while ( TRUE ) {
        // once a second
        now=time(NULL);
        if ( now != past ) {
            // blink green LED
            if ( ledGreen ) {
                arduino.setIoPortHigh(LED_GREEN);
            } else {
                arduino.setIoPortLow(LED_GREEN);
            }
            ledGreen = !ledGreen;
            past = now;
        }
        
        // Toggle red LED on button release
        inputPorts=arduino.getIoPorts();
        if ( buttonPressed ) {
            if ( inputPorts & BUTTON ) {
                if ( ledRed ) {
                    arduino.setIoPortLow(LED_RED);
                } else {
                    arduino.setIoPortHigh(LED_RED);
                }
                ledRed = !ledRed;
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
