
/*
	Pin Allocations
	===============
	
	0	-	Push button, pull-up with connection to ground when pressed
	
	1	-	Beeper with integral oscillator, active high
	
	2	-	Neopixels
	
	3	-	CS to SD breakout
	
	4	-	SDA to sensors
	
	5	-	SCL to sensors
	
	6	-	TX to GPS
	
	7	-	RX from GPS
	
	8	-	SCK to SD breakout
	
	9	-	MISO to SD breakout
	
	10	-	MOSI to SD breakout
	
	3v3	-	Power to SD breakout with its own regulator removed
	
	GND	-	General
	
	5v	-	Power to sensors with own regulators and GPS
*/

// for the sensors
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_BMP280.h>

// for the sd card
#include <SPI.h>








/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel=Adafruit_ADXL345_Unified(12345);


Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();


void displaySensorDetails(void)
{
	sensor_t sensor;
	accel.getSensor(&sensor);

	Serial.println("------------------------------------");
	Serial.print  ("Sensor:       "); Serial.println(sensor.name);
	Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
	Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
	Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
	Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
	Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
	Serial.println("------------------------------------");
	Serial.println("");

	delay(500);
}

void displayDataRate(void)
{
	Serial.print	("Data Rate:		"); 
	
	switch(accel.getDataRate())
	{
		case ADXL345_DATARATE_3200_HZ:	Serial.print("3200 "); 	break;
		case ADXL345_DATARATE_1600_HZ:	Serial.print("1600 ");	break;
		case ADXL345_DATARATE_800_HZ:	Serial.print("800 ");	break;
		case ADXL345_DATARATE_400_HZ:	Serial.print("400 ");	break;
		case ADXL345_DATARATE_200_HZ:	Serial.print("200 ");	break;
		case ADXL345_DATARATE_100_HZ:	Serial.print("100 ");	break;
		case ADXL345_DATARATE_50_HZ:	Serial.print("50 ");	break;
		case ADXL345_DATARATE_25_HZ:	Serial.print("25 ");	break;
		case ADXL345_DATARATE_12_5_HZ:	Serial.print("12.5 ");	break;
		case ADXL345_DATARATE_6_25HZ:	Serial.print("6.25 ");	break;
		case ADXL345_DATARATE_3_13_HZ:	Serial.print("3.13 ");	break;
		case ADXL345_DATARATE_1_56_HZ:	Serial.print("1.56 ");	break;
		case ADXL345_DATARATE_0_78_HZ:	Serial.print("0.78 ");	break;
		case ADXL345_DATARATE_0_39_HZ:	Serial.print("0.39 ");	break;
		case ADXL345_DATARATE_0_20_HZ:	Serial.print("0.20 ");	break;
		case ADXL345_DATARATE_0_10_HZ:	Serial.print("0.10 ");	break;
		
		default:						Serial.print("???? ");	break;
	}
	
	Serial.println(" Hz");	
}

void displayRange(void)
{
	Serial.print	("Range:				 +/- "); 
	
	switch(accel.getRange())
	{
		case ADXL345_RANGE_16_G:	Serial.print("16 ");	break;
		case ADXL345_RANGE_8_G:		Serial.print("8 ");		break;
		case ADXL345_RANGE_4_G:		Serial.print("4 ");		break;
		case ADXL345_RANGE_2_G:		Serial.print("2 ");		break;
		default:					Serial.print("?? ");	break;
	}	
	
	Serial.println(" g");	
}

void SetupButton(void)
{
}

void PollButton(void)
{
}

void SetupBeeper(void)
{
}

void PollBeeper(void)
{
}

void SetupNeopixels(void)
{
}

void PollNeopixels(void)
{
}

void SetupAccelerometer(void)
{
	Serial.println("Accelerometer Test"); Serial.println("");
	
	/* Initialise the sensor */
	if(!accel.begin())
	{
		/* There was a problem detecting the ADXL345 ... check your connections */
		Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
		while(1);
	}

	/* Set the range to whatever is appropriate for your project */
	accel.setRange(ADXL345_RANGE_16_G);
	// accel.setRange(ADXL345_RANGE_8_G);
	// accel.setRange(ADXL345_RANGE_4_G);
	// accel.setRange(ADXL345_RANGE_2_G);
	
	/* Display some basic information on this sensor */
	displaySensorDetails();
	
	/* Display additional settings (outside the scope of sensor_t) */
	displayDataRate();
	displayRange();
	Serial.println("");
}

void PollAccelerometer(void)
{
	/* Get a new sensor event */ 
	sensors_event_t event; 
	accel.getEvent(&event);
	
	/* Display the results (acceleration is measured in m/s^2) */
	Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("	");
	Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("	");
	Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("	");Serial.println("m/s^2 ");
}

void SetupPressureSensor(void)
{
	Serial.println(F("BMP280 Sensor event test"));
	
	unsigned status;
	
	status=bmp.begin();
	
	if(!status)
	{
		Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
											"try a different address!"));
		Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
		Serial.print("\tID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
		Serial.print("\tID of 0x56-0x58 represents a BMP 280,\n");
		Serial.print("\tID of 0x60 represents a BME 280.\n");
		Serial.print("\tID of 0x61 represents a BME 680.\n");
		while (1) delay(10);
	}

	/* Default settings from datasheet. */
	bmp.setSampling(
						Adafruit_BMP280::MODE_NORMAL,		/* Operating Mode. */
						Adafruit_BMP280::SAMPLING_X2,		/* Temp. oversampling */
						Adafruit_BMP280::SAMPLING_X16,		/* Pressure oversampling */
						Adafruit_BMP280::FILTER_X16,		/* Filtering. */
						Adafruit_BMP280::STANDBY_MS_500		/* Standby time. */
					);
	
	bmp_temp->printSensorDetails();
}

void PollPressureSensor(void)
{
}

void SetupGps(void)
{
}

void PollGps(void)
{
}
























void setup(void) 
{
	Serial.begin(115200);
	
	while ( !Serial ) delay(100);	 // wait for native usb
	
	SetupBeeper();
	SetupButton();
	SetupNeopixels();
	SetupAccelerometer();
	SetupPressureSensor();
	SetupGps();
}

void loop(void) 
{
	PollBeeper();
	PollButton();
	PollNeopixels();
	PollAccelerometer();
	PollPressureSensor();
	PollGps();
	
	delay(100);
}

