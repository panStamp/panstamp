#include "HardwareSerial.h"
#include "cc430i2c.h"
#include "mma8652.h"

CC430I2C i2c;
uint8_t i, buf[20];
uint16_t accelX, accelY, accelZ;

void configAccelerometer(void)
{
  // Place MMA8652 in standby 
  buf[0] = MMA8652_CTRL_REG1;
  buf[1] = 0;
  i2c.beginTransmission(MMA8652_CTRL_ID_DEFAULT);
  i2c.write(buf, 2);

  delay(1);

  // Accelerometer range of +/-2g range with 0.244mg/LSB
  buf[0] = MMA8652_XYZ_DATA_CFG;
  buf[1] = 0;
  i2c.beginTransmission(MMA8652_CTRL_ID_DEFAULT);
  i2c.write(buf, 2);

  delay(1);
  
  // Active=1 to take the part out of standby and enable sampling
  buf[0] = MMA8652_CTRL_REG1;
  buf[1] = 0x21;
  i2c.beginTransmission(MMA8652_CTRL_ID_DEFAULT);
  i2c.write(buf, 2);
}

void setup()
{
  i2c.begin();
  Serial.begin(9600);
  Serial.println("Starting...");
  
  configAccelerometer();
}

void loop()
{
  // Who Am I?
  buf[0] = MMA8652_WHO_AM_I;
  i2c.beginTransmission(MMA8652_CTRL_ID_DEFAULT);
  i2c.write(buf, 1, false);  // false = no stop bit
  i2c.read(buf, 1);
  Serial.print("Who am I : ");
  Serial.println(buf[0], HEX);
  
  // Read axis
  buf[0] = MMA8652_OUT_X_MSB;
  i2c.beginTransmission(MMA8652_CTRL_ID_DEFAULT);
  i2c.write(buf, 1, false);  // false = no stop bit
  i2c.read(buf, 6);
  
  accelX = ((buf[0] << 8) | buf[1]) >> 4;
  accelY = ((buf[2] << 8) | buf[3]) >> 4;
  accelZ = ((buf[4] << 8) | buf[5]) >> 4;
  
  Serial.print("X axis : ");
  Serial.println(accelX, HEX);
  Serial.print("Y axis : ");
  Serial.println(accelY, HEX);
  Serial.print("Z axis : ");
  Serial.println(accelZ, HEX);

  delay(1000);
}

