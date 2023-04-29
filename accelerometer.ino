#include <Wire.h>
#include <ADXL335.h>

ADXL335 adxl;
float x,y,z;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    adxl.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
    adxl.getAcceleration(&x, &y, &z);
    Serial.println(x);
}
