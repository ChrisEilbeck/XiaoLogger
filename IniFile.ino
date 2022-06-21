
#include <SD.h>
#include <IniFile.h>

const char *inifilename = "/config.ini";

// from the GPS section of the config file

char GPSType[16]="Generic";
int InitialBaudRate=9600;
int BaudRate=115200;
bool SyncTimeToGPS=true;
bool DoBaudRateChange=true;
bool DoHighFixRate=true;
int HighFixRate=10;
char ChangeBaudRateCommand[80];
char HighFixRateCommand[80];

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
	
	Serial.print("GPSType:\t\t");				Serial.println(GPSType);
	Serial.print("InitialBaudRate:\t");			Serial.println(InitialBaudRate);
	Serial.print("BaudRate:\t\t");				Serial.println(BaudRate);
	Serial.print("DoBaudRateChange:\t");		Serial.println(DoBaudRateChange?"True":"False");
	Serial.print("DoHighFixRate:\t\t");			Serial.println(DoHighFixRate?"True":"False");
	Serial.print("SyncTimeToGPS:\t\t");			Serial.println(SyncTimeToGPS?"True":"False");
	Serial.println("");
	Serial.print("ChangeBaudRateCommand:\t");	Serial.println(ChangeBaudRateCommand);
	Serial.print("HighFixRateCommand:\t");		Serial.println(HighFixRateCommand);
	Serial.println("");
	
	Serial.print("AccelType:\t\t");				Serial.println(AccelType);
	Serial.print("MeasurementRate:\t");			Serial.println(AccelMeasurementRate);
	Serial.println("");
	
	Serial.print("GyroType:\t\t");				Serial.println(GyroType);
	Serial.print("MeasurementRate:\t");			Serial.println(GyroMeasurementRate);
	Serial.println("");
	
	Serial.print("PressureType:\t\t");			Serial.println(PressureType);
	Serial.print("MeasurementRate:\t");			Serial.println(PressureMeasurementRate);
	Serial.print("SyncSamplingToGPS:\t");		Serial.println(PressureSyncSamplingToGPS?"True":"False");
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
	ini.getValue("GPS","SyncTimeToGPS",buffer,bufferLen,SyncTimeToGPS);
	ini.getValue("GPS","DoBaudRateChange",buffer,bufferLen,DoBaudRateChange);
	ini.getValue("GPS","BaudRate",buffer,bufferLen,BaudRate);
	ini.getValue("GPS","DoHighFixRate",buffer,bufferLen,DoHighFixRate);
	ini.getValue("GPS","ChangeBaudRateCommand",buffer,bufferLen,ChangeBaudRateCommand,sizeof(ChangeBaudRateCommand));
	ini.getValue("GPS","HighFixRateCommand",buffer,bufferLen,HighFixRateCommand,sizeof(HighFixRateCommand));
	
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
		opf.println("");
		
		opf.println("[Pressure]");
		opf.println("SyncSamplingToGPS=1");
		opf.println("");
		
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

