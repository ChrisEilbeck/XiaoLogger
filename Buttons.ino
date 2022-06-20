
#include "Buttons.h"

void SetupButtons(void)
{
	pinMode(BUTTON0,INPUT_PULLUP);
	pinMode(BUTTON1,INPUT_PULLUP);
}

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

