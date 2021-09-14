#include <Wire.h>

#define ds1621 0x90 >> 1                        // shift required by wire.h

void setup()
{
  Serial.begin(115200);

  Wire.begin();
  Wire.beginTransmission(ds1621);             // connect to DS1621 (#0)
  Wire.write(0xAC);                            // Access Config
  Wire.write(0x02);                            // set for continuous conversion
  Wire.beginTransmission(ds1621);             // restart
  Wire.write(0xEE);                            // start conversions
  Wire.endTransmission();
}


void loop()
{
  int8_t firstByte;
  int8_t secondByte;
  float temp = 0;

  delay(1000);                                // give time for measurement

  Wire.beginTransmission(ds1621);
  Wire.write(0xAA);                            // read temperature command
  Wire.endTransmission();
  Wire.requestFrom(ds1621, 2);    // request two bytes from DS1621 (0.5 deg. resolution)

  firstByte = Wire.read();           // get first byte
  secondByte = Wire.read();    // get second byte

  temp = firstByte;

  if (secondByte)             // if there is a 0.5 deg difference
    temp += 0.5;

  Serial.println(temp);
}
