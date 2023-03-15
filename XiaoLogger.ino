
#define DEBUG 1

#define SEALEVELPRESSURE_HPA 1013.0f

#define BUTTON0 0
#define BUTTON1 1

#define BEEPER 2

#include <Wire.h>
#include <SPI.h>

















bool normaloperation=true;

// controls the amount of logging that goes to the serial console

int verbose=1;

// sensors

#include "Accelerometer.h"
#include "PressureSensor.h"
#include "Gps.h"

// user interface

#include "Beeper.h"
#include "Buttons.h"
#include "SDCard.h"
#include "IniFile.h"
#include "OLEDDisplay.h"





#include <SD.h>







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
	// wait for serial connection from the host
	// do not use this apart from when debugging

	while(!Serial)
		delay(100);
#endif
}

char CmdBuffer[80];
int CmdBufferPtr=0;

void PollSerial(uint32_t now)
{
	if(Serial.available())
	{
		char rxbyte=Serial.read();
		
		Serial.write(rxbyte);
		
		if(rxbyte!=0x0d)
		{
			CmdBuffer[CmdBufferPtr++]=rxbyte;
		}
		else
		{
			CmdBuffer[CmdBufferPtr++]=0;
			
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
					Serial.println("Listing files ...");
					
					File root=SD.open("/");
					PrintDirectory(root,0);
				}
				else if(strncmp(CmdBuffer,"cat",3)==0)
				{
					// read a file from the sd card
					Serial.print("Cataloguing a file ");
					Serial.println(CmdBuffer+4);
					
					if(strlen(CmdBuffer)>4)
					{
						CatFile(CmdBuffer+4);
					}
					else
					{
						Serial.println("No filename supplied ...");
						CatFile("/CONFIG.INI");
					}
				}
				else if(strncmp(CmdBuffer,"rm",2)==0)
				{
					// delete a file from the sd card
					Serial.print("Deleting a file ...\r\n");
				}
				else if(strncmp(CmdBuffer,"writeini",8)==0)	{	WriteIniFile();																		}
				else if(strncmp(CmdBuffer,"readini",7)==0)	{	ReadIniFile();																		}
				else if(strncmp(CmdBuffer,"start",5)==0)	{	Serial.println("Starting logging ...");					sdcard_enable=true;			}
				else if(strncmp(CmdBuffer,"stop",4)==0)		{	Serial.println("Stopping logging ...");					sdcard_enable=false;		}
				else if(strncmp(CmdBuffer,"status",6)==0)	{	Serial.print("Logging is ");	Serial.println(sdcard_enable?"Enabled":"Disabled");	}
				else if(strncmp(CmdBuffer,"detailed",8)==0)	{	Serial.println("Setting detailed output to serial");	verbose=2;					}
				else if(strncmp(CmdBuffer,"verbose",7)==0)	{	Serial.println("Setting verbose output to serial");		verbose=1;					}
				else if(strncmp(CmdBuffer,"terse",5)==0)	{	Serial.println("Setting terse output to serial");		verbose=0;					}
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
#if 0
	SetupSerial();
	SetupSDCard();

	SetupIniFile();
	SetupBeeper();
	SetupButtons();
	SetupOLEDDisplay();

	SetupAccelerometer();
	SetupPressureSensor();
	SetupGps();
#else
	SetupSerial();
	SetupOLEDDisplay();
	SetupGps();
#endif
}

void loop(void) 
{
	uint32_t now=millis();
	
#if 0
	// poll the sensors.  these are higher priority than the other attached
	// devices so having them measured with the minimal delay from the
	// timestamp value is a good thing
	PollAccelerometer(now);
	PollPressureSensor(now);
#endif
#if 0
	// this is probably redundant
	PollSDCard(now);
#endif
#if 0
	// this is not fitted in the current version of the hardware
	PollBeeper(now);
#endif
#if 0
	PollButtons(now);
#endif
	
	PollOLEDDisplay(now);
	
	// respond to data received from the GPS
	PollGps(now);
	
	// respond to commands over the serial console link
	PollSerial(now);
	
#if 0
	// respond to events generated using the buttons
	PollCommandInterface(now);	
#endif
}

void PrintDirectory(File dir,int numTabs)
{
	while(true)
	{
		File entry=dir.openNextFile();
		
		if(!entry)
		{
			// no more files
			break;
		}
		
		for(uint8_t i=0;i<numTabs;i++)
		{
			Serial.print('\t');
		}
		
		Serial.print(entry.name());
		
		if(entry.isDirectory())
		{
			Serial.println("/");
			PrintDirectory(entry,numTabs+1);
		}
		else
		{
			// files have sizes, directories do not
			Serial.print("\t\t");
			Serial.println(entry.size(),DEC);
		}
		
		entry.close();
	}
}

void CatFile(char *filename)
{
	Serial.print("Reading from ");
	Serial.println(filename);
	
	File ipf=SD.open(filename);
	
	if(ipf)
	{
		while(ipf.available())
			Serial.write(ipf.read());
		
		ipf.close();
	}
	else
		Serial.println("Couldn't open file");
}

