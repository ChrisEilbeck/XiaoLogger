
#include <TimeLib.h>

#include "Gps.h"
#include "IniFile.h"
#include "PressureSensor.h"

TinyGPSPlus gps;

char linebuffer[80];
int lineptr=0;

double maxalt_gps=-1000.0;

int NibbleToValue(char nibble)
{
	if((nibble>='0')&&(nibble<='9'))		return(nibble-'0');
	else if((nibble>='a')&&(nibble<'f'))	return(nibble-'a'+10);
	else if((nibble>='A')&&(nibble<'F'))	return(nibble-'A'+10);
	else									return(0);
}

void SendHexMessage(char *hexmsg)
{
	Serial.print("Sending ");	Serial.print(strlen(hexmsg)/2);		Serial.println(" bytes to the GPS");

	char *binmsg=(char *)malloc(strlen(hexmsg)/2);

	for(int cnt=0;cnt<(strlen(hexmsg)/2);cnt++)
	{
		binmsg[cnt]=NibbleToValue(hexmsg[cnt*2])*16+NibbleToValue(hexmsg[cnt*2+1]);

		if(binmsg[cnt]<16)	Serial.print("0");
		Serial.print(binmsg[cnt],HEX);
	}
	
	Serial.println("");
	
	Serial1.write(binmsg,strlen(hexmsg)/2);
	
	free(binmsg);
}

void SetupGps(void)
{
	Serial1.begin(InitialBaudRate);

#if 0	
	if(strncmp(GPSType,"Neo8m",5)==0)
	{
		if(SetHighBaudRate)
		{
			// restart the serial port to run at a higher rate so we can process more 
			// messages and a higher fix rate
			
			SendHexMessage(HighBaudRateCommand);
			
			Serial1.end();
			delay(100);
			Serial1.begin(BaudRate);
		}	
	}
#endif
#if 0
	#if 0
			SendHexMessage("B5620600140001000000D0080000004B000023000300000000006437");
	#endif
	#if 1
			SendHexMessage("B5620600140001000000D0080000009600002300030000000000AF70");
	#endif
	
			delay(500);
			Serial1.end();
			delay(500);
			Serial1.begin(38400);
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
			{
				Serial.print(linebuffer);
			}
			
#if 0
			if(		(strncmp(linebuffer,"$GPGGA",6)==0)
				||	(strncmp(linebuffer,"$GNGGA",6)==0)	)
			{
				Serial.print(linebuffer);
			}
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




