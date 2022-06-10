
#define DEBUG 1

#define SEALEVELPRESSURE_HPA 1013.0f

#define BUTTON0 0
#define BUTTON1 1

#define BEEPER 2

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

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C




bool normaloperation=true;

// controls the amount of logging that goes to the serial console

int verbose=1;;

#include "Gps.h"
#include "SDCard.h"










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
	pinMode(BUTTON0,INPUT_PULLUP);
	pinMode(BUTTON1,INPUT_PULLUP);
}

enum BUTTON_EVENTS
{
	no_event=0,
	pressed,
	released,
	long_pressed
};

uint8_t button0event=no_event;
uint8_t button1event=no_event;

#define LONG_PRESS_TIME 1000

void PollButtons(uint32_t now)
{
	static uint32_t poll_timer=20;	// ms
	static uint32_t last_poll=0;
	
	static bool button0=true;
	static bool button1=true;
	
	static uint32_t button0timer=0;
	static uint32_t button1timer=0;
	
	if(now>(last_poll+poll_timer))
	{
		bool newbutton0=digitalRead(BUTTON0);
		bool newbutton1=digitalRead(BUTTON1);
		
		if(newbutton0&&button0)
		{
			// button still up
			button0event=no_event;
			
//			Serial.println("Button0 released");
		}
		else if(!newbutton0&&button0)
		{
			// button pressed
			button0timer=0;
			button0event=pressed;
			
			Serial.println("Button0 pressed");
		}
		else if(!newbutton0&!button0)
		{
			// button still pressed
			button0timer+=poll_timer;
			
//			Serial.println("Button0 held");
		}
		else
		{
			Serial.print("Button held for ");
			Serial.print(button0timer);
			Serial.println(" ms");
			
			// button released
			if(button0timer>=LONG_PRESS_TIME)
				button0event=long_pressed;
			else
				button0event=released;
			
			Serial.println("Button0 released");
		}
		
		if(newbutton1&&button1)
		{
			// button still up
			button1event=no_event;
		}
		else if(!newbutton1&&button1)
		{
			// button pressed
			button1timer=0;
			button1event=pressed;
		}
		else if(!newbutton1&!button1)
		{
			// button still pressed
			button1timer+=poll_timer;
		}
		else
		{
			// button released
			if(button1timer>=LONG_PRESS_TIME)
				button1event=long_pressed;
			else
				button1event=released;
		}
		
		button0=newbutton0;
		button1=newbutton1;
		
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
		
#if 0
		/* Display the results (acceleration is measured in m/s^2) */
		Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(", ");
		Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(", ");
		Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" ");		Serial.println("m/s^2 ");
#else
		char buffer[80];
		sprintf(buffer,"$ACXYZ,%f,%f,%f\r\n",event.acceleration.x,event.acceleration.y,event.acceleration.z);
		SDCardLogMessage(buffer);
#endif
		
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







void PollCommandInterface(uint32_t now)
{
	if(button0event!=no_event)
	{
		Serial.print("\tButton0 Event ");
		Serial.println(button0event);
	
		if(button0event==long_pressed)
		{
			display.clearDisplay();
			display.setTextSize(2);
			display.setTextColor(SSD1306_WHITE);
			display.setCursor(0,0);
			display.println("Stopping\r\nOperation\r\n");
			
			display.display();
			
			normaloperation=false;
		}	
			
		button0event=no_event;
	}
	
	if(button1event!=no_event)
	{
		Serial.print("\tButton1 Event ");
		Serial.println(button1event);
	
		if(button1event==long_pressed)
		{
			display.clearDisplay();
			display.setTextSize(2);
			display.setTextColor(SSD1306_WHITE);
			display.setCursor(0,0);
			display.println("Starting\r\nOperation\r\n");
			
			display.display();
			
			normaloperation=true;
		}	
			
		button1event=no_event;
	}
}

void SetupSerial(void)
{
	Serial.begin(115200);
	
#if 0
	while ( !Serial ) delay(100);	 // wait for native usb
#endif
}

char CmdBuffer[80];
int CmdBufferPtr=0;

void PollSerial(uint32_t now)
{
	if(Serial.available())
	{
		char rxbyte=Serial.read();
		CmdBuffer[CmdBufferPtr++]=rxbyte;
		
		Serial.write(rxbyte);
		
		if(rxbyte==0x0d)
		{
#if 0
			Serial.print("Command complete - ");
			Serial.print(CmdBuffer);
			Serial.print("\r\n");
#endif		
			if(CmdBufferPtr==1)
			{
				// empty command, ignore
				
				Serial.print("...\r\n");
			}
			else if(CmdBufferPtr==2)
			{
				// process single character commands
			}
			else
			{
				// process multi character commands
				
				if(strncmp(CmdBuffer,"ls",2)==0)
				{
					// List all the files on the sd card
					Serial.print("Listing files ...\r\n");
				}
				else if(strncmp(CmdBuffer,"cat",3)==0)
				{
					// read a file from the sd card
					Serial.print("Cataloguing a file ...\r\n");
				}
				else if(strncmp(CmdBuffer,"rm",2)==0)
				{
					// delete a file from the sd card
					Serial.print("Deleting a file ...\r\n");
				}
				else if(strncmp(CmdBuffer,"start",5)==0)	{	Serial.print("Starting logging ...\r\n");				sdcard_enable=true;		}
				else if(strncmp(CmdBuffer,"stop",4)==0)		{	Serial.print("Stopping logging ...\r\n");				sdcard_enable=false;	}
				else if(strncmp(CmdBuffer,"detailed",8)==0)	{	Serial.print("Setting detailed output to serial\r\n");	verbose=2;				}
				else if(strncmp(CmdBuffer,"verbose",7)==0)	{	Serial.print("Setting verbose output to serial\r\n");	verbose=1;				}
				else if(strncmp(CmdBuffer,"terse",5)==0)	{	Serial.print("Setting terse output to serial\r\n");		verbose=0;				}
				else
				{
					Serial.print("Duff command, ignoring ...\r\n");
				}
			}
			
			CmdBufferPtr=0;
		}
	}
}

void setup(void) 
{
	SetupSerial();
	SetupSDCard();

#if 0
	SetupBeeper();
#endif

	SetupButtons();
	SetupOLEDDisplay();
	SetupAccelerometer();
	SetupPressureSensor();
	SetupGps();
}

void loop(void) 
{
	uint32_t now=millis();
	
	// poll the sensors.  these are higher priority than the other attached
	// devices so having them measured with the minimal delay from the
	// timestamp value is a good thing
	PollAccelerometer(now);
	PollPressureSensor(now);

#if 0
	// this is probably redundant
	PollSDCard(now);
#endif
#if 0
	// this is not fitted in the current version of the hardware
	PollBeeper(now);
#endif

	PollButtons(now);
	PollOLEDDisplay(now);
	
	// respond to data received from the GPS
	PollGps(now);
	
	// respond to commands over the serial console link
	PollSerial(now);
	
	// respond to events generated using the buttons
	PollCommandInterface(now);	
}

