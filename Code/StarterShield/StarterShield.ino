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
#include "RTClib.h"

// 
#define ONE_WIRE_BUS 8 // Temperature probe data line
#define CS 10 // Chip Select on SD logger
#define ledPin 9 // LED

// Create the objects - what the actual sensor returns data values to
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temp(&oneWire);
// Maybe we should create an RTC object here?

// Declare the variables used in your sketch
float temperature = 0;
// Can you think of any other variables we will need throughout our sketch?

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
  
  // Does an RTC count as a sensor?
  
  pinMode(ledPin, OUTPUT); // Declare LED as an output pin
}

void loop() {

  // Call the sensors and have them return their data to the objects
  temp.requestTemperatures();
  //Any other data we should be retrieving? 

  // Assigning the values from the sensors to variables on the Arduino
  temperature = temp.getTempCByIndex(0);

  // To write to a file with this SD logger, you must first open the file
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // If the file is available (meaning that it could open the file from the SD card), write to it:
  if (dataFile) {
    dataFile.println(temperature);
    // Anything else we should save in the data file?
    dataFile.close();
  }

  // If the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

  // print to your computer too
  Serial.println(temperature);

  digitalWrite(ledPin, HIGH); // turn on the led
  delay(1000);
  
  digitalWrite(ledPin, LOW); // turn off the LED
  delay(1000);
}
