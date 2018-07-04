    /******************************************************************************
  Log GPS data to a CSV file on a uSD card
  https://github.com/anishasbhat/TrackMe

  This example uses SoftwareSerial to communicate with the GPS module on
  pins 8 and 9, then communicates over SPI to log that data to a uSD card.

  It uses the TinyGPS++ library to parse the NMEA strings sent by the GPS module,
  and prints interesting GPS information - comma separated - to a newly created
  file on the SD card.

  Resources:
  TinyGPS++ Library  - https://github.com/mikalhart/TinyGPSPlus/releases
  SD Library (Built-in), SoftwareSerial Library (Built-in)

  Development/hardware environment specifics:
  Arduino IDE 1.6.7
  GPS Logger Shield v2.0 - UART switch is set to SW-UART, Arduino Uno
******************************************************************************/
//Libraries:
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Log File Defintions: 
#define FILE_NAME "gpslog" // Name of the log file.
#define MAX_FILE_NUM 100 // Max number of files that can be made
#define FILE_SUFFIX "csv" // file type
char logFileName[13]; // Char string to store the log file name

//Destination Definitions:
const double DEST_LAT = 35.689487;
const double DEST_LNG = 139.691706;//2.294516;

// Data setup:
#define COLUMN_COUNT 10 //columns of data in the file
const char * col_names[COLUMN_COUNT] = {
  "longitude", "latitude", "altitude", "speed", "course", "date", "time", "satellites",
  "dist to destination", "Human Directions"}; //data headers

// Log Rate:
#define LOG_RATE 5000 // Log every 5 seconds
unsigned long lastLog = 0; // keep track of last time info was logged

// TinyGPS Definitions:
TinyGPSPlus tinyGPS; // tinyGPSPlus object
#define GPS_BAUD 9600 // GPS's baud rate

// GPS Serial Port Definitions:
#define ARDUINO_USD_CS 10 // uSD card CS pin (pin 10)
#define ARDUINO_GPS_RX 9 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 8 // GPS RX, Arduino TX pin
SoftwareSerial ss(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
#define gpsPort ss  //gpsPort set to Software Serial
#define SerialMonitor Serial //Serial Monitor Port

void setup()
{
  SerialMonitor.begin(GPS_BAUD);
  gpsPort.begin(GPS_BAUD);
  
  initializeCard();
  createFile(); //creates new file + adds headers
}

void loop()
{
  logData();

  //"feed" tinyGPS object when data isn't being logged
  while (gpsPort.available())
  tinyGPS.encode(gpsPort.read());
}

byte saveData()
{
  File logFile = SD.open(logFileName, FILE_WRITE); // Open the log file

    double distanceKm =
  tinyGPS.distanceBetween(
    tinyGPS.location.lat(),
    tinyGPS.location.lng(),
    DEST_LAT,
   DEST_LNG) / 1000.0;

    double courseTo =
  tinyGPS.courseTo(
    DEST_LAT,
    DEST_LNG,tinyGPS.location.lat(),
    tinyGPS.location.lng());

  if (logFile)
  { // Print longitude, latitude, altitude (in feet), speed (in mph), course
    // in (degrees), date, time, and number of satellites.
    logFile.print(tinyGPS.location.lng(), 6);
    logFile.print(',');
    logFile.print(tinyGPS.location.lat(), 6);
    logFile.print(',');
    logFile.print(tinyGPS.altitude.feet(), 1);
    logFile.print(',');
    logFile.print(tinyGPS.speed.mph(), 1);
    logFile.print(',');
    logFile.print(tinyGPS.course.deg(), 1);
    logFile.print(',');
    
    //Date
    logFile.print(tinyGPS.date.month());
    logFile.print("/");
    logFile.print(tinyGPS.date.day());
    logFile.print("/");
    logFile.print(tinyGPS.date.year());
    logFile.print(',');
    
    //Time- Greenwich Mean Time (GMT)
    logFile.print(tinyGPS.time.hour());
    logFile.print(":");
    if (tinyGPS.time.minute() < 10) logFile.print('0');
    logFile.print(tinyGPS.time.minute());
    logFile.print(":");
    if (tinyGPS.time.second() < 10) logFile.print('0');
    logFile.print(tinyGPS.time.second()); 
    logFile.print(',');
    logFile.print(tinyGPS.satellites.value());
    logFile.print(',');
    logFile.print(distanceKm);
    logFile.print(',');
    logFile.print(tinyGPS.cardinal(courseTo));
    
    logFile.println();
    logFile.close(); //close file

    return 1; // return success
  }
  return 0; // If we failed to open the file, return fail
}

// printHeader() - prints our eight column names to the top of our log file
void printHeader()
{
  File logFile = SD.open(logFileName, FILE_WRITE); // Open the log file

  if (logFile) // If the log file opened, print our column names to the file
  {
    int i = 0;
    for (; i < COLUMN_COUNT; i++)
    {
      logFile.print(col_names[i]);
      if (i < COLUMN_COUNT - 1) // If not last column
        logFile.print(','); // print a comma
      else logFile.println(); // print a new line
    }
    logFile.close(); // close file
  }
}

//creates a new file with an incremented file index.
void updateFileName()
{
  int i = 0;
  for (; i < MAX_FILE_NUM; i++)
  {
    memset(logFileName, 0, strlen(logFileName)); // Clear logFileName string
    // Set logFileName to "gpslogXX.csv":
    sprintf(logFileName, "%s%d.%s", FILE_NAME, i, FILE_SUFFIX);
    if (SD.exists(logFileName)) // If a file exists
    {
      SerialMonitor.print(logFileName);
      SerialMonitor.println(" exists"); // Print a debug statement
    }
    else
    {
       break; // Break. We found our index
    }
  }
  SerialMonitor.print("File name: ");
  SerialMonitor.println(logFileName); // Debug print the file name
}

void initializeCard()
{
  SerialMonitor.println("Setting up SD card.");

  if (SD.begin(ARDUINO_USD_CS)) //if card is present
  {
     SerialMonitor.println("Card initialized!");
    }
  else{
    SerialMonitor.println("Error initializing SD card.");
  }
  }

void createFile()
{
  updateFileName(); // Each time we start, create a new file, increment the number
  printHeader(); // Print a header at the top of the new file
}

//sets Date to MM/DD/YY format 
void printDate()
{
  File logFile = SD.open(logFileName, FILE_WRITE); // Open the log file
   if (logFile)
  {
     
    //Date
    logFile.print(tinyGPS.date.month());
    logFile.print("/");
    logFile.print(tinyGPS.date.day());
    logFile.print("/");
    logFile.print(tinyGPS.date.year());
  }
}

//sets time to EST
void printTime()
{
  File logFile = SD.open(logFileName, FILE_WRITE); // Open the log file
  if (logFile)
  {
    logFile.print(tinyGPS.time.hour());
    logFile.print(":");
    if (tinyGPS.time.minute() < 10) logFile.print('0');
    logFile.print(tinyGPS.time.minute());
    logFile.print(":");
    if (tinyGPS.time.second() < 10) logFile.print('0');
    logFile.print(tinyGPS.time.second()); 
  }
}

void logData()
{// checks if data should be logged, then logs data
  
  if ((lastLog + LOG_RATE) <= millis()) // If it's been LOG_RATE milliseconds since last log
  { 
    if (tinyGPS.location.isUpdated()) // If GPS data is vaild
    {
      if (saveData()) // if data is logged
      {
        SerialMonitor.println("GPS logged."); 
        lastLog = millis(); // Update lastLog variable
      }
      else
      { 
        SerialMonitor.println("Failed to log new GPS data.");
      }
    }
    else // If GPS data isn't valid
    {
      SerialMonitor.print("No GPS data. Sats: ");
      SerialMonitor.println(tinyGPS.satellites.value());
    }
  }
}

void directions()
{
  
  double distanceKm =
  tinyGPS.distanceBetween(
    tinyGPS.location.lat(),
    tinyGPS.location.lng(),
    DEST_LAT,
   DEST_LNG) / 1000.0;

    double courseTo =
  tinyGPS.courseTo(
    DEST_LAT,
    DEST_LNG,tinyGPS.location.lat(),
    tinyGPS.location.lng());
    
  File logFile = SD.open(logFileName, FILE_WRITE); // Open the log file
  if (logFile)
  {
       logFile.print(distanceKm);
       logFile.print(tinyGPS.cardinal(courseTo));
  }
}

