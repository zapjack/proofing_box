
// put in github!!!!
/**
Author: Alan Jackson
Date:   26 April 2023

Controller for a bread and yogurt proofing box.
The controller reads the temperature every TIME_LAPSE seconds, and compares the current temperature
to the desired temperature. It turns an incandescent light bulb on or off if the current temperature drifts 
more than TOLERANCE degrees from the desired temperature.

The user moves a slider switch to select proofing for either bread or yogurt.

Some temperature sensors are not that well calibrated. Use the CALIBRATE value to adjust the reported
values from the temperature sensor to real world values.
*/

#include <time.h>

const float TEMP_BREAD = 84.0;
const float TEMP_YOGURT = 110.0;
const float CALIBRATE = -10.0;  // Set this to 0.0 if temperature sensor is accurate. Otherwise use this to callibrate the sensor.

float     desiredTemp = TEMP_BREAD;
int       switchState;          // Positions are BREAD or YOGURT
const int BREAD = LOW;
const int YOGURT = HIGH;

const float TOLERANCE = 2.0;    // how many degrees actual temp drifts from desired temp before light bulb goes on or off
const int   TIME_LAPSE = 5000;  // time between temperature measurements, 1000 = 1sec

const int SWITCH_PIN = 3;       // toggles between bread and yogurt, LOW == bread, HIGH == yogurt
const int HEAT_PIN = 7;         // relay switch to light bulb
const int READ_TEMP_PIN = A5;   // reads the temperature sensor

int count = 0;

void setup() {
  Serial.begin(9600);
  pinMode(HEAT_PIN, OUTPUT);          // controls the relay to the light bulb
  pinMode(SWITCH_PIN, INPUT_PULLUP);  // switch to select bread or yogurt
}

void loop() {
  controlBulb(getTemp());
  delay(TIME_LAPSE);
}

// For temperatureC, converting from 10 mv per degree with 500 mV offset 
// to degrees ((voltage - 500mV) x 100)

float getTemp() {
  int temp = analogRead(READ_TEMP_PIN);
  float voltage = temp * 5.0;
  voltage /= 1024.0;
  float temperatureC = (voltage - 0.5) * 100;
  float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  return temperatureF + CALIBRATE;
}

void controlBulb(float temp) {
  Serial.print(temp); Serial.println(" degrees");

  if (temp > desiredTemp + TOLERANCE)
    digitalWrite(HEAT_PIN, LOW);

  if (temp < desiredTemp - TOLERANCE)
    digitalWrite(HEAT_PIN, HIGH);
}

