#include <Wire.h>
#include <BH1750.h>
#include <ADXL335.h>

#define THRESHOLD 100
#define DATA_SIZE 256   // change the int types to comply with data size
#define ON 255
#define OFF 0
#define PIN9 9
#define x_min 0
#define x_max 2.5

BH1750 sensor;
ADXL335 adxl;
float x, y, z;

uint16_t data[DATA_SIZE];
float average;
uint16_t *current;

float calc_avg(uint16_t arr[], int n);

void setup()
{
    Serial.begin(9600);

    Wire.begin();
    adxl.begin();
    
    if (sensor.begin())
        Serial.println("----- Starting -----");
    else {
        Serial.println("----- Error -----");
        while(1);
    }
    
    average = 0;
    // initialize data array to some initial reading
    for (unsigned int i = 0; i < DATA_SIZE; ++i) {
        data[i] = sensor.readLightLevel();
        average += (float) (unsigned int) data[i];
    }
    average /= DATA_SIZE;
    current = data;
}

void loop()
{  
    average -= (float) (unsigned int) *current / DATA_SIZE;
    *current = sensor.readLightLevel();
    average += (float) (unsigned int) *current / DATA_SIZE;

    if (average < THRESHOLD) {
        analogWrite(PIN9, ON);
        delay(10 - average / 10);
        analogWrite(PIN9, OFF);
        delay(average / 10);
    }

    if (++current == data + DATA_SIZE) {
        do {
            adxl.getAcceleration(&x, &y, &z);
        } while (x_min < x && x < x_max);
        current = data;
        average = calc_avg(data, DATA_SIZE);
    }
}

float calc_avg(uint16_t arr[], int n) {
    float avg = 0;
    uint16_t *temp = arr;
    do {
        avg += *temp;
    } while (++temp != arr + n);
    return avg / n;
}
