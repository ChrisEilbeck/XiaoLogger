
#pragma once

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

void displaySensorDetails(void);
void displayDataRate(void);
void displayRange(void);

void SetupAccelerometer(void);
void PollAccelerometer(uint32_t now);
