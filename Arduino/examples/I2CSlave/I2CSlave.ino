#include <Wire.h>
#include <Servo.h>
#include <I2CSlave.h>
#include <I2CSlaveCommon.h>

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
  unsigned char count=0;
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
