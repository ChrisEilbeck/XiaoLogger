
/*
	Pin Allocations
	===============
	
	0	-	Push button, pull-up with connection to ground when pressed
	
	1	-	Push button, pull-up with connection to ground when pressed
	
	2	-	Beeper with integral oscillator, active high
	
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

#define DEBUG 1

#define SEALEVELPRESSURE_HPA (1012.0)

#include <Wire.h>
#include <SPI.h>

// for the sensors
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_BMP280.h>

// for the sd card

// for the i2c OLED display

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH,SCREEN_HEIGHT,&Wire,OLED_RESET);








/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel=Adafruit_ADXL345_Unified(12345);

//Adafruit_BMP280 bmp;
//Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
//Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C







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

void SetupButtons(void)
{
}

void PollButtons(uint32_t now)
{
	static uint32_t poll_timer=20;	// ms
	static uint32_t last_poll=0;
	
	if(now>(last_poll+poll_timer))
	{
		
		
		
		
		
		last_poll=now;
	}	
}

static uint32_t beeper_sequence=0x00;
static bool beeper_repeat=false;

void SetupBeeper(void)
{
}

void PollBeeper(uint32_t now)
{
	static uint32_t poll_timer=100;	// ms
	static uint32_t last_poll=0;
	
	if(now>(last_poll+poll_timer))
	{
		
		
		
		
		
		last_poll=now;
	}	
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
	//accel.setRange(ADXL345_RANGE_16_G);
	//accel.setRange(ADXL345_RANGE_8_G);
	//accel.setRange(ADXL345_RANGE_4_G);
	accel.setRange(ADXL345_RANGE_2_G);
	
	/* Display some basic information on this sensor */
	displaySensorDetails();
	
	/* Display additional settings (outside the scope of sensor_t) */
	displayDataRate();
	displayRange();
	Serial.println("");
}

void PollAccelerometer(uint32_t now)
{
	static uint32_t poll_timer=200;	// ms
	static uint32_t last_poll=0;
	
	if(now>(last_poll+poll_timer))
	{
		/* Get a new sensor event */ 
		sensors_event_t event; 
		accel.getEvent(&event);
		
		/* Display the results (acceleration is measured in m/s^2) */
		Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(", ");
		Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(", ");
		Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" ");		Serial.println("m/s^2 ");
		
		last_poll=now;
	}	
}

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

void PollPressureSensor(uint32_t now)
{
	static uint32_t poll_timer=1000;	// ms
	static uint32_t last_poll=0;
	
	if(now>(last_poll+poll_timer))
	{
#if 0
		Serial.print("Temperature = ");
		Serial.print(bme.readTemperature());
		Serial.println(" °C");

		Serial.print("Pressure = ");
		Serial.print(bme.readPressure() / 100.0F);
		Serial.println(" hPa");

		Serial.print("Approx. Altitude = ");
		Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
		Serial.println(" m");

		Serial.print("Humidity = ");
		Serial.print(bme.readHumidity());
		Serial.println(" %");
#else
		Serial.print("T: ");	Serial.print(bme.readTemperature());					Serial.print(" C, ");
		Serial.print("P: ");	Serial.print(bme.readPressure());						Serial.print(" hPa, ");
		Serial.print("A: ");	Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));	Serial.print(" m, ");
		Serial.print("H: ");	Serial.print(bme.readHumidity());						Serial.println(" %");

#endif

		Serial.println();
		
		last_poll=now;
	}	
}

#include <TinyGPS++.h>

TinyGPSPlus gps;

char linebuffer[80];
int lineptr=0;








void SetupGps(void)
{
	Serial1.begin(9600);
	




}

void PollGps(uint32_t now)
{
//	Serial.print("\x1b[2J");

#if 0
	char linebuffer[80];
	int lineptr=0;
#endif	
#if 0
	if (Serial.available()) 
	{ 
		// If anything comes in Serial (USB),
		Serial1.write(Serial.read());	 // read it and send it out Serial1 (pins 0 & 1)
	}

	if (Serial1.available()) 
	{
		// If anything comes in Serial1 (pins 0 & 1)
		Serial.write(Serial1.read());	 // read it and send it out Serial (USB)
	}
#else
	while(Serial1.available())
	{
		char rxbyte=Serial1.read();
		
//		Serial.write('.');
		
		gps.encode(rxbyte);
		
		linebuffer[lineptr++]=rxbyte;
		
		if(linebuffer[lineptr-1]=='\n')
		{
			if(strstr(linebuffer,"GPGLL")!=NULL)
			{
			}
			
			lineptr=0;
			memset(linebuffer,0,sizeof(linebuffer));
		}
		
	}	
	
	
	
	
	
	
	
	
	
#endif
}










void SetupOLEDDisplay()
{
	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if(!display.begin(SSD1306_SWITCHCAPVCC,SCREEN_ADDRESS))
	{
		Serial.println(F("SSD1306 display allocation failed, hanging ..."));
		
		// hang forever
		while(1);
	}

	// Show initial display buffer contents on the screen --
	// the library initializes this with an Adafruit splash screen.
	display.display();
	delay(2000); // Pause for 2 seconds
}

void PollOLEDDisplay(uint32_t now)
{
	static uint32_t poll_timer=20;	// ms
	static uint32_t last_poll=0;
	
	if(now>(last_poll+poll_timer))
	{
		display.clearDisplay();

		display.setTextSize(1);             // Normal 1:1 pixel scale
		display.setTextColor(SSD1306_WHITE);        // Draw white text

		float flat;
		float flon;
		unsigned long age;
				
		display.setCursor(0,0);             // Start at top-left corner

		display.print("Lat: ");
		display.println(gps.location.lat(),6);

		display.print("Lon: ");
		display.println(gps.location.lng(),6);		
		
		display.print("Alt: ");
		display.println(gps.altitude.meters());

		display.print("Numsats: ");
		display.println(gps.satellites.value());
		
		display.display();
		
		last_poll=now;
	}
}














void setup(void) 
{
	Serial.begin(115200);
	
#if 0
	while ( !Serial ) delay(100);	 // wait for native usb
#endif
	
//	SetupSDCard();
//	SetupBeeper();
//	SetupButtons();
	SetupOLEDDisplay();
	SetupAccelerometer();
	SetupPressureSensor();
	SetupGps();
}

void loop(void) 
{
	uint32_t now=millis();
	
//	PollSDCard(now);
//	PollBeeper(now);
//	PollButtons(now);
	PollOLEDDisplay(now);
	PollAccelerometer(now);
	PollPressureSensor(now);
	PollGps(now);
}

