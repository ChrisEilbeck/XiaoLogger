
#include <SD.h>
#include <IniFile.h>

const char *inifilename = "/config.ini";

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

void SetupIniFile(void)
{
	IniFile ini(inifilename);
  
	if(!ini.open())
	{
		Serial.print("Ini file ");
		Serial.print(inifilename);
		Serial.println(" does not exist");
    
		// Cannot do anything else
		while(1)	{	}
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
	
		// Cannot do anything else
		while(1)	{	}
	}
	
	
	
	
	
	
	
	

}

void PollIniFile(uint32_t now)
{

}

