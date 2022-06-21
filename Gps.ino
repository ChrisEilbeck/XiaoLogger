
#define FIX_RATE_5HZ 0
#define BAUD_RATE_CHANGE 0

#include <TimeLib.h>

#include "Gps.h"
#include "IniFile.h"
#include "PressureSensor.h"

TinyGPSPlus gps;

char linebuffer[80];
int lineptr=0;

double maxalt_gps=-1000.0;

void SetupGps(void)
{
	Serial1.begin(9600);
	
#if BAUD_RATE_CHANGE
	// restart the serial port to run at a higher rate so we can process more 
	// messages and a higher fix rate
	Serial1.end();
	delay(100);
	Serial1.begin(9600);
//	Serial1.begin(38400);
#endif
}

void PollGps(uint32_t now)
{
//	Serial.print("\x1b[2J");

#if 0
	while(Serial1.available())
	{
		char rxbyte=Serial1.read();
		Serial.write(rxbyte);
	}
	
	while(Serial.available())
	{
		char rxbyte=Serial.read();
		Serial1.write(rxbyte);
	}
#else
	while(Serial1.available())
	{
		char rxbyte=Serial1.read();
		
		gps.encode(rxbyte);
		
		linebuffer[lineptr++]=rxbyte;
		
		if(linebuffer[lineptr-1]=='\n')
		{
			// process a full line of NMEA from the GPS
			
			if(		(strncmp(linebuffer,"$GPGLL",6)==0)
				||	(strncmp(linebuffer,"$GNGLL",6)==0)	)
			{
				if(maxalt_gps<gps.altitude.meters())
					maxalt_gps=gps.altitude.meters();
			}
			
			SDCardLogMessage(linebuffer);

			if(verbose>0)
				Serial.print(linebuffer);
			
#if 0
			if(strncmp(linebuffer,"$GPGGA",6)==0)
				Serial.print(linebuffer);
#endif
			
			if(		SyncTimeToGPS
				&&	(strncmp(linebuffer,"$GPRMC",6)==0)	)
			{
				Serial.println("Setting time from the GPS");
				setTime(gps.time.hour(),gps.time.minute(),gps.time.second(),gps.date.day(),gps.date.month(),gps.date.year());
			}
			
			if(		PressureSyncSamplingToGPS
				&&	(		(strncmp(linebuffer,"$GPGGA",6)==0)
						||	(strncmp(linebuffer,"$GNGGA",6)==0)		)	)
			{
				BaroSampleNow=true;
			}
			
			lineptr=0;
			memset(linebuffer,0,sizeof(linebuffer));
		}
	}
#endif
}




