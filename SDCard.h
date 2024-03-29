
#pragma once

#include "SD.h"
#include "SPI.h"

extern bool sdcard_enable;

#define SDCARD_NSS 3
#define SDCARD_SCK 8
#define SDCARD_MISO 9
#define SDCARD_MOSI 10

int SetupSDCard(void);

void PollSDCard(uint32_t now);

void SDCardLogMessage(const char *logmessage);


