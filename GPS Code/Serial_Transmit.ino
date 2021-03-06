/******************************************************************************
  TinyGPS++ Library Example for the SparkFun GPS Logger Shield
  June 26, 2018
  https://github.com/anishasbhat/GPS_Tracker

  This example uses SoftwareSerial to communicate with the GPS module on
  pins 8 and 9. It uses the TinyGPS++ library to parse NMEA strings sent
  by the GPS module an transmits the corresponding information onto the
  serial monitor.

  Setup + Resources:
  TinyGPS++ Library  - https://github.com/mikalhart/TinyGPSPlus/releases
  SoftwareSerial Library

  Development/hardware environment specifics:
  Arduino IDE 1.6.7, Arduino Uno.
  GPS Logger Shield v2.0 - UART switch is set to SW-UART
******************************************************************************/

//Libraries
#include <TinyGPS++.h> // Include the TinyGPS++ library
TinyGPSPlus gps; // Create a TinyGPSPlus object

//Definitions
#define GPS_BAUD 9600 // GPS module baud rate. 
#include <SoftwareSerial.h> //Used for Arduino Uno
#define ARDUINO_GPS_RX 9 // GPS TX, Arduino RX pin
#define ARDUINO_GPS_TX 8 // GPS RX, Arduino TX pin

SoftwareSerial ss(ARDUINO_GPS_TX, ARDUINO_GPS_RX); // Create a SoftwareSerial
#define gpsPort ss  // Set gpsPort to ss as SoftwareSerial is used
#define SerialMonitor Serial // Define the serial monitor port


void setup()
{
  SerialMonitor.begin(GPS_BAUD);
  gpsPort.begin(GPS_BAUD);
}

void loop()
{
  printGPSInfo(); // print position, altitude, speed, time/date, and satellites:
  smartDelay(1000);  // looks for GPS data while the Arduino's not doing anything else
}

void printGPSInfo()
{
  // Print latitude, longitude, altitude in feet, course, speed, date, time,
  // and the number of visible satellites.
  SerialMonitor.print("Lat: "); SerialMonitor.println(gps.location.lat(), 6);
  SerialMonitor.print("Long: "); SerialMonitor.println(gps.location.lng(), 6);
  SerialMonitor.print("Alt: "); SerialMonitor.println(gps.altitude.feet());
  SerialMonitor.print("Course: "); SerialMonitor.println(gps.course.deg());
  SerialMonitor.print("Speed: "); SerialMonitor.println(gps.speed.mph());
  SerialMonitor.print("Date: "); printDate();
  SerialMonitor.print("Time: "); printTime();
  SerialMonitor.print("Sats: "); SerialMonitor.println(gps.satellites.value());
  SerialMonitor.println();
}

//ensures that the gps object is being "fed"
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (gpsPort.available()) // If data has come in from the GPS module
      gps.encode(gpsPort.read()); // Send it to the encode function
  } while (millis() - start < ms);
}

// printDate() formats the date into mm/dd/yy.
void printDate()
{
  SerialMonitor.print(gps.date.month());
  SerialMonitor.print("/");
  SerialMonitor.print(gps.date.day());
  SerialMonitor.print("/");
  SerialMonitor.println(gps.date.year());
}

// printTime() formats the time into "hh:mm:ss"
void printTime()
{
  SerialMonitor.print(gps.time.hour());
  SerialMonitor.print(":");
  if (gps.time.minute() < 10) SerialMonitor.print('0');
  SerialMonitor.print(gps.time.minute());
  SerialMonitor.print(":");
  if (gps.time.second() < 10) SerialMonitor.print('0');
  SerialMonitor.println(gps.time.second());
}
