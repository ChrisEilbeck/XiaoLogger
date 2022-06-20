
#include "Beeper.h"

uint32_t beeper_sequence=0x00;
bool beeper_repeat=false;

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


