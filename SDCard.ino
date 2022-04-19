
#include "SDCard.h"

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
	
	Serial.print("\nInitializing SD card...");

#if 0
	if(!SD.begin(SDCARD_NSS))
	{
		Serial.println("Card Startup Failed");
		return(1);
	}
	
	uint8_t cardType=SD.cardType();
	if(cardType==CARD_NONE)
	{
		Serial.println("No SD card attached");
		return(1);
	}
	
	Serial.print("SD Card Type: ");
	if(cardType==CARD_MMC)
	{
		Serial.println("MMC");
	}
	else if(cardType==CARD_SD)
	{
		Serial.println("SDSC");
	}
	else if(cardType==CARD_SDHC)
	{
		Serial.println("SDHC");
	}
	else
	{
		Serial.println("UNKNOWN");
	}
	
	uint64_t cardSize = SD.cardSize() / (1024 * 1024);
	Serial.printf("SD Card Size: %lluMB\r\n\n", cardSize);
#else

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
	root.openRoot(volume);

	// list all files in the card with date and size
	root.ls(LS_R | LS_DATE | LS_SIZE);
	
#endif

	return(0);
}

void PollSDCard(uint32_t now)
{

}

