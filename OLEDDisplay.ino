
#include "OLEDDisplay.h"

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
	delay(1000); // Pause for 2 seconds
}

void PollOLEDDisplay(uint32_t now)
{
	static uint32_t poll_timer=1000;	// ms
	static uint32_t last_poll=0;
	static uint16_t screencount=0;
	
	if(now>(last_poll+poll_timer))
	{
		if(normaloperation)
		{
			display.clearDisplay();
			
			// Normal 1:1 pixel scale
			display.setTextSize(1);// Normal 1:1 pixel scale
			
			// draw white on black if logging is active, inverted otherwise
			if(sdcard_enable)	display.setTextColor(SSD1306_WHITE);
			else				display.setTextColor(SSD1306_INVERSE);
			
			display.setCursor(0,0);
			
			char buffer[32];
			sprintf(buffer,"%04d/%02d/%02d %02d:%02d:%02d\r\n",gps.date.year(),gps.date.month(),gps.date.day(),gps.time.hour(),gps.time.minute(),gps.time.second());
			display.print(buffer);
			
			switch(screencount&0x0f)
			{
				case 0 ... 5:	display.setTextSize(1);
								display.println();
								display.print("Lat: ");
								display.println(gps.location.lat(),6);
								display.print("Lon: ");
								display.println(gps.location.lng(),6);		
								
								break;
				
				case 6 ... 7:	display.println();
								display.print("# Sats: ");
								display.println(gps.satellites.value());
								
								break;
				
				case 8 ... 9:	display.println();
								display.print("Alt(GPS):\r\n  ");
								display.print(gps.altitude.meters());
								display.println(" m");
				
								break;
								
				case 10 ... 11:	display.println();
								display.print("Max(GPS):\r\n  ");
								display.print(maxalt_gps);
								display.println(" m");
								
								break;
				
				case 12 ... 13:	display.println();
								display.print("Alt(Baro):\r\n  ");
								display.print(curalt_baro);
								display.println(" m");
								
								break;
				
				case 14 ... 15:	display.println();
								display.print("Max(Baro):\r\n  ");
								display.print(maxalt_baro);
								display.println(" m");
								
								break;
				
				default:		// do nothing
								break;
			}
			
			display.setTextSize(3);
			display.setCursor(88,10);
			if(gps.satellites.value()<3)		display.println("NF");
			else if(gps.satellites.value()==3)	display.println("2D");
			else								display.println("3D");
			
			display.display();
			
			screencount++;
		}
		
		last_poll=now;
	}
}

