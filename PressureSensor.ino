
#include "PressureSensor.h"

Adafruit_BME280 bme;

void SetupPressureSensor(void)
{
	Serial.println(F("BME280 Sensor event test"));
	
	unsigned status;
	
	status=bme.begin(0x76);
	
	if(!status)
	{
		Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
											"try a different address!"));
		Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
		Serial.print("\tID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
		Serial.print("\tID of 0x56-0x58 represents a BMP 280,\n");
		Serial.print("\tID of 0x60 represents a BME 280.\n");
		Serial.print("\tID of 0x61 represents a BME 680.\n");
		while (1) delay(10);
	}

#if 0
	/* Default settings from datasheet. */
	bme.setSampling(
						Adafruit_BMP280::MODE_NORMAL,		/* Operating Mode. */
						Adafruit_BMP280::SAMPLING_X2,		/* Temp. oversampling */
						Adafruit_BMP280::SAMPLING_X16,		/* Pressure oversampling */
						Adafruit_BMP280::FILTER_X16,		/* Filtering. */
						Adafruit_BMP280::STANDBY_MS_500		/* Standby time. */
					);
#endif
	
//	bme_temp->printSensorDetails();
}

double curalt_baro=-1000.0;
double maxalt_baro=-1000.0;

void PollPressureSensor(uint32_t now)
{
	static uint32_t poll_timer=1000;	// ms
	static uint32_t last_poll=0;
	
	if(now>(last_poll+poll_timer))
	{
#if 0
		Serial.print("T: ");	Serial.print(bme.readTemperature());					Serial.print(" C, ");
		Serial.print("P: ");	Serial.print(bme.readPressure());						Serial.print(" hPa, ");
		Serial.print("A: ");	Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));	Serial.print(" m, ");
		Serial.print("H: ");	Serial.print(bme.readHumidity());						Serial.println(" %");

		Serial.println();		
#else
		char buffer[80];
		sprintf(buffer,"$PTPAH,%f,%f,%f,%f\r\n",bme.readTemperature(),bme.readPressure(),bme.readAltitude(SEALEVELPRESSURE_HPA),bme.readHumidity());
		SDCardLogMessage(buffer);		
#endif

		curalt_baro=bme.readAltitude(SEALEVELPRESSURE_HPA);
		
		if(maxalt_baro<curalt_baro)
			maxalt_baro=curalt_baro;
		
		last_poll=now;
	}	
}






