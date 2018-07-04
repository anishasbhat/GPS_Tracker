#include <SD.h> //SD Card Library
#include <TinyGPS++.h>  //GPS library
#include <SoftwareSerial.h> //Serial Monitor
#include <SPI.h>

TinyGPSPlus tinyGPS;
const int GPS_BAUD = 9600;

#define ARDUINO_GPS_RX 9 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 8 // GPS RX, Arduino TX pin
SoftwareSerial ss(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
#define gpsPort ss
#define SerialMonitor Serial


void setup() {
  SerialMonitor.begin(GPS_BAUD);
  SerialMonitor.println("Welcome to TrackMe! Type 'MORE' for more information!");
  gpsPort.begin(GPS_BAUD);

}

void loop() {
  String response = Serial.readString();
  SerialMonitor.println(response);

  if(response.equalsIgnoreCase("MORE"))
    {
      SerialMonitor.println("Type 'LAT' for current latitude");
      SerialMonitor.println("Type 'LONG' for current longitude");
      SerialMonitor.println("Type 'SPD' for current speed");
      SerialMonitor.println("Type 'ALT' for current altitude");
      SerialMonitor.println("Type 'CRS' for current course");
      SerialMonitor.println("Type 'TIME' for current time (GST)");
      SerialMonitor.println("Type 'DATE' for current date");
      SerialMonitor.println("Type 'NAV' for directions to a location");
      SerialMonitor.println("Type 'SAT' to see how many satellities you are connected to!");
      SerialMonitor.println("Type 'MORE' to see this screen again");
       //Save to SD Card
    }//Latitude
    else if(response.equalsIgnoreCase("LAT")){
      SerialMonitor.println("Your current latitude is: ");
      SerialMonitor.print(tinyGPS.location.lat(), 6);
    }//Longitude
    else if(response.equalsIgnoreCase("LONG")){
      SerialMonitor.println("Your current longitude is: ");
      SerialMonitor.print(tinyGPS.location.lng(), 6);
    }//Speed
    else if(response.equalsIgnoreCase("SPD")){
      SerialMonitor.println("Your current speed is: ");
      SerialMonitor.print(tinyGPS.speed.mph());
    }//Altitude
    else if(response.equalsIgnoreCase("ALT")){
      SerialMonitor.println("Your current altitude is: ");
      SerialMonitor.print(tinyGPS.altitude.feet());
    }//Course
    else if(response.equalsIgnoreCase("CRS")){
      SerialMonitor.println("Your current course is: ");
      SerialMonitor.print(tinyGPS.course.deg());
    }//Time
    else if(response.equalsIgnoreCase("TIME")){
      SerialMonitor.println("The current time is: ");
      printTime();
    }//Date
    else if(response.equalsIgnoreCase("DATE")){
      SerialMonitor.println("Today is: ");
      printDate();
    }//Navigation
    else if(response.equalsIgnoreCase("NAV")){
      /*
      float lat = Serial.parseFloat();
      SerialMonitor.println("latitude: ");
      SerialMonitor.print(lat);
       float long = Serial.parseFloat();
       SerialMonitor.println("longitude: ");
       SerialMonitor.print(long);
      */
    
    }//Satellites
    else if(response.equalsIgnoreCase("SAT")){
      SerialMonitor.println("You are connected to ");
      SerialMonitor.print(tinyGPS.satellites.value());
      SerialMonitor.print(" satellites");
    }
  smartDelay(1000); 
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    // If data has come in from the GPS module
    while (gpsPort.available())
      tinyGPS.encode(gpsPort.read()); // Send it to the encode function
    // tinyGPS.encode(char) continues to "load" the tinGPS object with new
    // data coming in from the GPS module. As full NMEA strings begin to come in
    // the tinyGPS library will be able to start parsing them for pertinent info
  } while (millis() - start < ms);
}

// printDate() formats the date into dd/mm/yy.
void printDate()
{
  SerialMonitor.print(tinyGPS.date.month());
  SerialMonitor.print("/");
  SerialMonitor.print(tinyGPS.date.day());
  SerialMonitor.print("/");
  SerialMonitor.print(tinyGPS.date.year());
}

// printTime() formats the time into "hh:mm:ss", and prints leading 0's
// where they're called for.
void printTime()
{
  SerialMonitor.print(tinyGPS.time.hour());
  SerialMonitor.print(":");
  if (tinyGPS.time.minute() < 10) SerialMonitor.print('0');
  SerialMonitor.print(tinyGPS.time.minute());
  SerialMonitor.print(":");
  if (tinyGPS.time.second() < 10) SerialMonitor.print('0');
  SerialMonitor.print(tinyGPS.time.second());
}

