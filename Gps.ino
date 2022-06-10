
#define FIX_RATE_5HZ 0
#define BAUD_RATE_CHANGE 0

#include "Gps.h"

TinyGPSPlus gps;

char linebuffer[80];
int lineptr=0;

double maxalt_gps=-1000.0;

void SetupGps(void)
{
	Serial1.begin(9600);
	
#if BAUD_RATE_CHANGE
	// this is straight out of u-center 8.27.  it changes the first interface port to
	// use nmea and ubx at 115200 baud.
//	uint8_t cmdbaud[]={	0xB5,0x62,0x06,0x00,0x14,0x00,0x01,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0xC2,0x01,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0xF1,0xCE	};
	
	if(0)
	{
		// uart1 nmea+ubx in and out at 38400
		uint8_t cmdbaud[]="$PUBX,41,1,3,3,38400,0*24\r\n";
		Serial1.write(cmdbaud,sizeof(cmdbaud));
	}
		
	if(0)
	{
		// uart1 nmea+ubx in and out at 115200
		uint8_t cmdbaud[]="$PUBX,41,1,3,3,115200,0*1c\r\n";
		Serial1.write(cmdbaud,sizeof(cmdbaud));
	}
	
	if(0)
	{
		// uart2 nmea+ubx in and out at 115200
		uint8_t cmdbaud[]={	0xB5,0x62,0x06,0x00,0x14,0x00,0x02,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0xC2,0x01,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0xF5,0xFA	};
		Serial1.write(cmdbaud,sizeof(cmdbaud));
	}
#endif
#if FIX_RATE_5HZ
	// this sets the fix rate to 5Hz
	uint8_t cmdfixrate[]={	0xB5,0x62,0x06,0x08,0x06,0x00,0xC8,0x00,0x01,0x00,0x01,0x00,0xDE,0x6A	};
	Serial1.write(cmdfixrate,sizeof(cmdfixrate));
#else
	// this sets the fix rate to `Hz
	uint8_t cmdfixrate[]={	0xB5,0x62,0x06,0x08,0x06,0x00,0xE8,0x03,0x01,0x00,0x01,0x00,0x01,0x39 	};
	Serial1.write(cmdfixrate,sizeof(cmdfixrate));
#endif
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
			if(strstr(linebuffer,"GPGLL")!=NULL)
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
					
			lineptr=0;
			memset(linebuffer,0,sizeof(linebuffer));
		}
	}
#endif
}




