#include <I2CSlaveCommon.h> // include this fairly early
#include <Wire.h>
#include <Servo.h>
#include <I2CSlave.h>

#ifdef USE_SONAR
#include <NewPing.h>
#endif

// Slave Address for this device
#define SLAVE_ADDRESS         0x04

void setup() {
#ifdef I2C_DEBUG
  Serial.begin(115200);
#endif

  // Register Arduino as I2C Slave
  I2CSlave.begin(SLAVE_ADDRESS);
}

void loop() {
  static unsigned char count=0;
  // use counter as status byte and increas every 5 seconds
  count++;
  I2CSlave.setStatusByte(count);

#ifdef I2C_DEBUG
  Serial.print("[");
  Serial.print(count);
  Serial.println("]");
#endif
  delay(5000);
}
