/* This code is designed for the Starter Shields for the NearSpace ENAE100 Workshops
 *  Sensors incorperated on the shield
 *    DS18B22
 *    SD Logger (Adafruit)
 *    DS1307 (Adafruit)
 *  Code originally  written and commented by Tyler Boyle, September 2017
 *  Code edited by Blaire Weinberg, January 2019
 *  Email bweinbe1@umd.edu with further questions
 */

// Libraries
#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "RTClib.h" //install RTClib by Adafruit, compatible with DS1307

// 
#define ONE_WIRE_BUS 8 // Temperature probe data line
#define CS 10 // Chip Select on SD logger
#define ledPin 9 // LED

// Create the objects - what the actual sensor returns data values to
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temp(&oneWire);
RTC_DS1307 rtc;

// Declare the variables used in your sketch
float temperature = 0;

// We use int below because days are represented by decimals
// Floats provide decimals, while ints provide whole numbers
int year = 0;
int month = 0;
int day = 0;
int hour = 0;
int minute = 0;
int second = 0;

void setup() {
  Serial.begin(9600);

  // Make sure the logger is hooked up. If it's not nothing will run after this statement
  Serial.print("Initializing SD card...");

  if (!SD.begin(CS)) {
    Serial.println("Card failed, or not present");
  }

  Serial.println("card initialized.");

  // Initialize the sensors and get them to begin
  temp.begin();
  rtc.begin();

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Automatically set the real-time clock
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0)); // Manually set the real-time clock

  pinMode(ledPin, OUTPUT); // Declare LED as an output pin
}

void loop() {

  // Call the sensors and have them return their data to the objects
  temp.requestTemperatures();
  DateTime now = rtc.now();

  // Assigning the values from the sensors to variables on the Arduino
  temperature = temp.getTempCByIndex(0);

  // Proper scientific date formatting is yyyymmdd
  int time[6] = {now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second()};

  // To write to a file with this SD logger, you must first open the file
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // If the file is available (meaning that it could open the file from the SD card), write to it:
  if (dataFile) {
    for(int i = 0; i < 6; i++) 
    dataFile.println(time[i]);
    dataFile.println(temperature);
    dataFile.close();
  }

  // If the file isn't open, pop up an error:
  else {
    for(int i = 0; i < 6; i++) 
    Serial.println(time[i]);
    Serial.println("error opening datalog.txt");
    Serial.println(temperature);
  }

  // print to your computer too
  for(int i = 0; i < 6; i++)
  dataFile.println(time[i]);
  dataFile.println(temperature);

  digitalWrite(ledPin, HIGH); // turn on the led
  delay(1000);
  
  digitalWrite(ledPin, LOW); // turn off the LED
  delay(1000);
}
