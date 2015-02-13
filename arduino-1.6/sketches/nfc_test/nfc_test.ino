#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

Adafruit_NFCShield_I2C nfc(2, 3);

void setup()
{
  Serial.begin(38400);
  Serial.println("Hello!");
  
  
  nfc.begin();
/*  
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
*/  

  int i;
  uint8_t buf[26];
  uint8_t checksum;
  
  Wire.begin();
  Wire.beginTransmission((uint8_t)PN532_I2C_ADDRESS);
  checksum = PN532_PREAMBLE + PN532_PREAMBLE + PN532_STARTCODE2;
  Wire.write(PN532_PREAMBLE);
  Wire.write(PN532_PREAMBLE);
  Wire.write(PN532_STARTCODE2);
  Wire.write(0x01);//len
  Wire.write(~0x01 + 1);//~len+1
 
  Wire.write(PN532_HOSTTOPN532);
  checksum += PN532_HOSTTOPN532;
  
  Wire.write(PN532_COMMAND_GETFIRMWAREVERSION);
  
  checksum += PN532_COMMAND_GETFIRMWAREVERSION;
  
  Wire.write(~checksum);
  Wire.write(PN532_POSTAMBLE);
  Wire.endTransmission(false);
    
  //delay(55);
  
  Wire.requestFrom((uint8_t)PN532_I2C_ADDRESS, (uint8_t)14, true);
  Wire.read();
  
  Serial.print(F("Reading: "));
  for(i=0 ; i<14 ; i++)
  {
    delay(1);
    buf[i] = Wire.read();
    Serial.print(F(" 0x"));
    Serial.print(buf[i], HEX);
  }
  Serial.println();
}

void loop()
{
  delay(1000);
}
