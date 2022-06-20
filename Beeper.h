
#pragma once

extern uint32_t beeper_sequence;
extern bool beeper_repeat;

void SetupBeeper(void);
void PollBeeper(uint32_t now);
