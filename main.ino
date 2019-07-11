#include <SPI.h>
#include <SD.h>

const int SD_CHIP_PIN = 4;

File log_file;
char* LOG_FILENAME = "log-fil.txt"; // 8 characters only before the file extension!

int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 1000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

/**
 * Function copied online from Arduino website
 */
void check_SD_card()
{
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  Serial.print("\nInitializing SD card...");

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, SD_CHIP_PIN)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  // print the type of card
  Serial.println();
  Serial.print("Card type:         ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    while (1);
  }

  Serial.print("Clusters:          ");
  Serial.println(volume.clusterCount());
  Serial.print("Blocks x Cluster:  ");
  Serial.println(volume.blocksPerCluster());

  Serial.print("Total Blocks:      ");
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("Volume type is:    FAT");
  Serial.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  Serial.print("Volume size (Kb):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb):  ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Gb):  ");
  Serial.println((float)volumesize / 1024.0);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}

void write_to_SD(int value)
{
  log_file.print(value);
  log_file.print(value);
  log_file.flush();
}

void setup() 
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // This function will check whether SD card is recognised and print some info
  check_SD_card(); 

  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(SD_CHIP_PIN)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("Card initialized!");

  if(SD.exists(LOG_FILENAME))
  {
    Serial.println("FILE EXISTS, PLEASE CHANGE THE NAME");
    while(1);
  }

  log_file = SD.open(LOG_FILENAME, FILE_WRITE); //O_WRITE | O_CREAT | O_APPEND
  if (!log_file) {
    Serial.println("Some problem opening the log file " + String(LOG_FILENAME));
    while (1);
  }

  pinMode(8,INPUT);
}


void loop() 
{
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  if ((millis()-starttime) >= sampletime_ms) //if the sampel time = = 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; 
    Serial.print("Concentration = ");
    Serial.print(concentration);
    Serial.println(" pcs/0.01cf");
    Serial.println("\n");
    write_to_SD(concentration);
    lowpulseoccupancy = 0;
    starttime = millis();
  }

}
