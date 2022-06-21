
#include <SD.h>
#include <IniFile.h>

const char *inifilename="/config.ini";

// from the GPS section of the config file

char GPSType[16]="Generic";
int InitialBaudRate=9600;
int BaudRate=115200;
bool SyncTimeToGPS=true;
bool SetHighBaudRate=true;
bool SetHighFixRate=true;
bool SetKinematicsFilter=true;
int HighFixRate=10;
char HighBaudRateCommand[80];
char HighFixRateCommand[80];
char KinematicsFilterCommand[80];

// from the Accelerometer section of the config file
char AccelType[16]="ADXL345";
int AccelMeasurementRate=5;

// from the Gyro section of the config file
char GyroType[16]="None";
int GyroMeasurementRate=5;

// from the Pressure section of the config file
char PressureType[16]="BME280";
int PressureMeasurementRate=10;
bool PressureSyncSamplingToGPS=true;

// from the Logging section of the config file
bool UseUniqueFilenames=true;

void PrintConfigVariables(void)
{
	Serial.println("Config variables");
	Serial.println("");
	
	Serial.print("GPSType:\t\t\t");				Serial.println(GPSType);
	Serial.print("InitialBaudRate:\t\t");			Serial.println(InitialBaudRate);
	Serial.print("BaudRate:\t\t\t");				Serial.println(BaudRate);
	Serial.print("SyncTimeToGPS:\t\t\t");			Serial.println(SyncTimeToGPS?"True":"False");
	Serial.print("SetHighBaudRate:\t\t");			Serial.println(SetHighBaudRate?"True":"False");
	Serial.print("SetHighFixRate:\t\t\t");		Serial.println(SetHighFixRate?"True":"False");
	Serial.print("SetKinematicsFilter:\t\t");		Serial.println(SetKinematicsFilter?"True":"False");
	Serial.println("");
	Serial.print("ChangeBaudRateCommand:\t\t");	Serial.println(HighBaudRateCommand);
	Serial.print("HighFixRateCommand:\t\t");		Serial.println(HighFixRateCommand);
	Serial.print("KinematicsFilterCommand:\t");	Serial.println(KinematicsFilterCommand);
	Serial.println("");
	
	Serial.print("AccelType:\t\t\t");				Serial.println(AccelType);
	Serial.print("MeasurementRate:\t\t");			Serial.println(AccelMeasurementRate);
	Serial.println("");
	
	Serial.print("GyroType:\t\t\t");				Serial.println(GyroType);
	Serial.print("MeasurementRate:\t\t");			Serial.println(GyroMeasurementRate);
	Serial.println("");
	
	Serial.print("PressureType:\t\t\t");			Serial.println(PressureType);
	Serial.print("MeasurementRate:\t\t");			Serial.println(PressureMeasurementRate);
	Serial.print("SyncSamplingToGPS:\t\t");		Serial.println(PressureSyncSamplingToGPS?"True":"False");
	Serial.println("");
}

void printErrorMessage(uint8_t e, bool eol = true)
{
	switch(e)
	{
		case IniFile::errorNoError:			Serial.print("no error");				break;
		case IniFile::errorFileNotFound:	Serial.print("file not found");			break;
		case IniFile::errorFileNotOpen:		Serial.print("file not open");			break;
		case IniFile::errorBufferTooSmall:	Serial.print("buffer too small");		break;
		case IniFile::errorSeekError:		Serial.print("seek error");				break;
		case IniFile::errorSectionNotFound:	Serial.print("section not found");		break;
		case IniFile::errorKeyNotFound:		Serial.print("key not found");			break;
		case IniFile::errorEndOfFile:		Serial.print("end of file");			break;
		case IniFile::errorUnknownError:	Serial.print("unknown error");			break;

		default:							Serial.print("unknown error value");	break;
	}
  
	if(eol)
		Serial.println();
}

void ReadIniFile(void)
{
	Serial.print("Reading from ini file \"/config.ini\"\r\n");
	
	IniFile ini(inifilename);
	
	if(!ini.open())
	{
		Serial.print("Ini file ");
		Serial.print(inifilename);
		Serial.println(" does not exist");
		Serial.print("Using default parameters ...\r\n");
		return;
	}
  
	Serial.println("Ini file exists");

	// Check the file is valid.  This can be used to warn if any lines are
	// longer than the buffer.
	
	const size_t bufferLen=80;
	char buffer[bufferLen];

	if(!ini.validate(buffer, bufferLen))
	{
		Serial.print("ini file ");
		Serial.print(ini.getFilename());
		Serial.print(" not valid: ");
		printErrorMessage(ini.getError());
		Serial.println("Using default parameters ...");
		return;
	}
	
	Serial.println("Reading Ini file /config.ini");
	
	// GPS variables
	
	ini.getValue("GPS","Type",buffer,bufferLen,GPSType,sizeof(GPSType));
	ini.getValue("GPS","InitialBaudRate",buffer,bufferLen,InitialBaudRate);
	ini.getValue("GPS","BaudRate",buffer,bufferLen,BaudRate);
	ini.getValue("GPS","SyncTimeToGPS",buffer,bufferLen,SyncTimeToGPS);
	ini.getValue("GPS","SetHighBaudRate",buffer,bufferLen,SetHighBaudRate);
	ini.getValue("GPS","SetHighFixRate",buffer,bufferLen,SetHighFixRate);
	ini.getValue("GPS","SetKinematicsFilter",buffer,bufferLen,SetKinematicsFilter);
	ini.getValue("GPS","HighBaudRateCommand",buffer,bufferLen,HighBaudRateCommand,sizeof(HighBaudRateCommand));
	ini.getValue("GPS","HighFixRateCommand",buffer,bufferLen,HighFixRateCommand,sizeof(HighFixRateCommand));
	ini.getValue("GPS","KinematicsFilterCommand",buffer,bufferLen,KinematicsFilterCommand,sizeof(KinematicsFilterCommand));
	
	// Accelerometer variables

	ini.getValue("Accelerometer","Type",buffer,bufferLen,AccelType,sizeof(AccelType));
	ini.getValue("Accelerometer","MeasurementRate",buffer,bufferLen,AccelMeasurementRate);
	
	// Gyro variables
	
	ini.getValue("Gyro","Type",buffer,bufferLen,GyroType,sizeof(GyroType));
	ini.getValue("Gyro","MeasurementRate",buffer,bufferLen,GyroMeasurementRate);
	
	// Pressure sensor variables
	
	ini.getValue("Pressure","Type",buffer,bufferLen,PressureType,sizeof(PressureType));
	ini.getValue("Pressure","MeasurementRate",buffer,bufferLen,PressureMeasurementRate);
	ini.getValue("Pressure","SyncSamplingToGPS",buffer,bufferLen,PressureSyncSamplingToGPS);
	
	// Logging variables
	
	ini.getValue("Logging","UseUniqueFilenames",buffer,bufferLen,UseUniqueFilenames);

	PrintConfigVariables();
}


void WriteIniFile(void)
{
	Serial.print("Writing to ini file \"/config.ini\"\r\n");
	
	if(SD.exists("/config.ini"))
		SD.remove("/config.ini");
	
	File opf=SD.open("/config.ini",FILE_WRITE);
	
	if(opf)
	{
		opf.println("[GPS]");
		opf.println("Type=Neo8m");
		opf.println("InitialBaudRate=9600");
		opf.println("BaudRate=115200");
		opf.println("SyncTimeToGPS=1");
		opf.println("SetHighBaudRate=1");
		opf.println("SetHighFixRate=1");
		opf.println("HighBaudRateCommand=00000000");
		opf.println("HighFixRateCommand=11111111");
		opf.println("KinematicsFilterCommand=22222222");
		opf.println("");
		opf.println("[Accelerometer]");
		opf.println("Type=ADXL345");
		opf.println("MeasurementRate=100");
		opf.println("");
		opf.println("[Gyro]");
		opf.println("Type=None");
		opf.println("MeasurementRate=100");
		opf.println("");
		opf.println("[Pressure]");
		opf.println("Type=BME280");
		opf.println("MeasurementRate=10");
		opf.println("SyncSamplingToGPS=1");
		opf.println("");
		opf.println("[Logging]");
		opf.println("UseUniqueFilenames=1");
				
		opf.close();
	}
	else
	{
		Serial.println("Can't write to /config.ini");
	}
}

void SetupIniFile(void)
{
	ReadIniFile();
}

void PollIniFile(uint32_t now)
{

}

