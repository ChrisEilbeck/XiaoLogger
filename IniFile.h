
#pragma once

void SetupIniFile(void);
void PollIniFile(uint32_t now);

// from the GPS section of the config file

extern char GpsType[16];
extern int InitialBaudRate;
extern bool SyncTimeToGPS;
extern bool DoBaudRateChange;
extern int BaudRate;
extern bool DoHighFixRate;
extern int HighFixRate;
extern char ChangeBaudRateCommand[80];
extern char HighFixRateCommand[80];

// from the Accelerometer section of the config file
extern char AccelType[16];
extern int AccelMeasurementRate;

// from the Gyro section of the config file
extern char GyroType[16];
extern int GyroMeasurementRate;

// from the Pressure section of the config file
extern char PressureType[16];
extern int PressureMeasurementRate;
extern bool PressureSyncSamplingToGPS;

// from the Logging section of the config file
extern bool UseUniqueFilenames;

