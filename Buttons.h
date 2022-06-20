
#pragma once

extern uint8_t button0event;
extern uint8_t button1event;

void SetupButtons(void);
void PollButtons(uint32_t now);

enum BUTTON_EVENTS
{
	no_event=0,
	pressed,
	released,
	long_pressed
};

