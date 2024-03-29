
#include "SDCard.h"

//#define LOG_TO_A_NEW_FILE_EACH_TIME 1

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

bool sdcard_enable=true;

int SetupSDCard(void)
{
//	SPI.begin(SCK,MISO,MOSI);
	
	// I don't know if this is vital but it's definitely recommended if you're 
	// not usig the standard SPI SS pin, which we're not.
//	pinMode(SDCARD_NSS,OUTPUT);
	
#if 0
	Serial.print("\nInitializing SD card...");
	
	// we'll use the initialization code from the utility libraries
	// since we're just testing if the card is working!
  
	if(!card.init(SPI_HALF_SPEED,SDCARD_NSS))
	{
    	Serial.println("initialization failed. Things to check:");
    	Serial.println("* is a card inserted?");
    	Serial.println("* is your wiring correct?");
    	Serial.println("* did you change the chipSelect pin to match your shield or module?");
    	while (1);
	}
	else 
	{
		Serial.println("Wiring is correct and a card is present.");
	}

	// print the type of card
	Serial.println();
	Serial.print("Card type:         ");

	switch (card.type()) 
	{
		case SD_CARD_TYPE_SD1:		Serial.println("SD1");		break;
		case SD_CARD_TYPE_SD2:		Serial.println("SD2");		break;
		case SD_CARD_TYPE_SDHC:		Serial.println("SDHC");		break;
		
		default:					Serial.println("Unknown");
	}

	// Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
	if(!volume.init(card))
	{
		Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
		while (1);
	}
#else
	Serial.print("Initializing SD card...");
	
	if(!SD.begin(SDCARD_NSS))
	{
		Serial.println("initialization failed, hanging!");
		while(1);
	}
  
	Serial.println("initialization done.");
#endif
	
	return(0);
}

void PollSDCard(uint32_t now)
{
#if 0
	Serial.print("Clusters:          ");		Serial.println(volume.clusterCount());
	Serial.print("Blocks x Cluster:  ");		Serial.println(volume.blocksPerCluster());

	Serial.print("Total Blocks:      ");		Serial.println(volume.blocksPerCluster() * volume.clusterCount());	Serial.println();

	// print the type and size of the first FAT-type volume
	uint32_t volumesize;
	Serial.print("Volume type is:    FAT");		Serial.println(volume.fatType(), DEC);

	volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
	volumesize *= volume.clusterCount();       // we'll have a lot of clusters
	volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)

	Serial.print("Volume size (Kb):  ");		Serial.println(volumesize);
	Serial.print("Volume size (Mb):  ");		Serial.println((float)volumesize/1024.0);
	Serial.print("Volume size (Gb):  ");		Serial.println((float)volumesize/1024.0/1024.0);

	Serial.println("\nFiles found on the card (name, date and size in bytes): ");
	




	delay(5000);
#endif
}

void SDCardLogMessage(const char *logmessage)
{
#if DEBUGTOSERIAL
	Serial.print(logmessage);
#endif

	File myFile;
	
	if(sdcard_enable)
	{
#if LOG_TO_A_NEW_FILE_EACH_TIME
		char filename[80];
#else
		myFile=SD.open("logger.txt",FILE_WRITE);
#endif
		
		// if the file opened okay, write to it:
		if(myFile)
		{
			myFile.print(logmessage);
			myFile.close();
		}
		else
		{
			// if the file didn't open, print an error:
			Serial.println("error opening logger.txt");
		}
	}
}

