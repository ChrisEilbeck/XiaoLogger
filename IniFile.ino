
#include <SD.h>
#include <IniFile.h>

const char *inifilename = "/config.ini";

// from the GPS section of the config file

char GpsType[16]="Generic";
int InitialBaudRate=9600;
bool SyncTimeToGPS=true;
bool DoBaudRateChange=true;
int BaudRate=115200;
bool DoHighFixRate=true;
int HighFixRate=10;
bool DoOutputRawData=true;
char ChangeBaudRateCommand[80];
char HighFixRateCommand[80];
char OutputRawDataCommand[80];

// from the Accelerometer section of the config file
char AccelType[16]="ADXL345";
int AccelMeasurementRate=5;

// from the Gyro section of the config file
char GyroType[16]="None";
int GyroMeasurementRate=5;

// from the Pressure section of the config file
char PressureType[16]="BME280";
int PressureMeasurementRate=10;
bool SyncSamplingToGPS=true;

// from the Logging section of the config file
bool UseUniqueFilenames=true;

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

