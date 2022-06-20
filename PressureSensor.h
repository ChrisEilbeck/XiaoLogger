
#pragma once

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//#include <Adafruit_BMP280.h>

extern double maxalt_baro;
extern double curalt_baro;

void SetupPressureSensor(void);
void PollPressureSensor(uint32_t now);


