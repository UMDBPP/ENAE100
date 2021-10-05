/* This code is designed for the Starter Shields for the NearSpace ENAE100 Workshops
 *  Sensors incorperated on the shield
 *    DS18B22
 *    SD Logger (Adafruit)
 *    DS1307 (Adafruit)
 *  Code originally  written and commented by Tyler Boyle, September 2017
 *  Code modified by Blaire Weinberg, Fall 2019
 *  Code modified by Michael Kalin, Fall 2021
 */

// Libraries
#include <SPI.h>
#include <SD.h>
#include <OneWire.h> //install OneWire by Paul Stoffregen
#include <DallasTemperature.h> //install DallasTemperature by Miles Burton
#include <RTClib.h> //install RTClib by Adafruit

// Define constants which specify which pins we are using on our Arduino
#define ONE_WIRE_BUS 8 // Temperature probe data line
#define CS 10 // Chip Select on SD logger
#define ledPin 9 // LED

// Create the objects - what the actual sensor returns data values to
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temp(&oneWire);
RTC_DS1307 rtc; //make sure this RTC object matches the RTC you are using! (some people might be using a PCF8523)

void setup() {
  Serial.begin(9600);

  // Make sure the logger is hooked up. If it's not nothing will run after this statement
  Serial.print("Initializing SD card...");

  if (!SD.begin(CS)) {
    Serial.println("Card failed, or not present");
  }
  else {
    Serial.println("card initialized.");
  }

  // Initialize the sensors and get them to begin
  temp.begin();
  rtc.begin();

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Automatically set the real-time clock, when to resetting the time of your RTC make sure to remove its battery
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0)); // Manually set the real-time clock

  pinMode(ledPin, OUTPUT); // Declare LED as an output pin
}

void loop() {
  // Call the sensors and have them return their data to the objects
  temp.requestTemperatures();
  DateTime now = rtc.now();

  // Assigning the values from the sensors to variables on the Arduino
  float temperature = temp.getTempCByIndex(0);

  //Store the current time in variables that we will be able to easily call upon later
  int day = now.day(), month = now.month(), year = now.year(), hour = now.hour(), minute = now.minute(), second = now.second();

  // To write to a file with this SD logger, you must first open the file
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // If the file is available (meaning that it could open the file from the SD card), write to it:
  if (dataFile) {
    // This is a lot of repetetive code just to print the time...
    // We could certainly turn it into a function if we're feeling ambitious! See: https://www.arduino.cc/en/Reference/FunctionDeclaration
    if(month < 10)
      dataFile.print("0");
    dataFile.print(month);
    dataFile.print("/"); // Since we are printing single characters (data type of char) we could use '' instead of "", but for simplicity we will stick with double quotes
    if(day < 10) // Our RTC returns the time as an int, which means it wont have leading zeros when we print it, we add them in so our formatting stays consistant
      dataFile.print("0");
    dataFile.print(day);
    dataFile.print("/");
    dataFile.print(year);
    dataFile.print(" ");
    if(hour < 10)
      dataFile.print("0");
    dataFile.print(hour);
    dataFile.print(":");
    if(minute < 10)
      dataFile.print("0");
    dataFile.print(minute);
    dataFile.print(":");
    if(second < 10)
      dataFile.print("0");
    dataFile.print(second);
    dataFile.print(", "); // End each section of data with a comma, so our output is in the comma-separated-value (.csv) format
    
    dataFile.print(temperature); 
    // Each line in our datafile now has two entries, the first entry represents the time and the second entry represents the temperature at that time
    // When we attach more sensors to our Arduino, we can uncomment the line below to add another comma, and then print the data from the sensor
    // For each new piece of data you want to print, simply add a comma at the end of the previous data entry
    // dataFile.print(', ');
    
    dataFile.println(); // End each data take (row of data) with a new line
    
    dataFile.close(); //It is good practice to close the datafile when you are done using it, we will open it up again when we loop back to this section of the code
  }
  
  // If the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

  // Print to your computer too, if its connected
  //The code below is the same code as above, except we are using Serial.print() to print to our computer instead of dataFile.print() which prints to the datafile
  if(Serial) {
    if(month < 10)
      Serial.print("0");
    Serial.print(month);
    Serial.print("/");
    if(day < 10)
      Serial.print("0");
    Serial.print(day);
    Serial.print("/");
    Serial.print(year);
    Serial.print(" ");
    if(hour < 10)
      Serial.print("0");
    Serial.print(hour);
    Serial.print(":");
    if(minute < 10)
      Serial.print("0");
    Serial.print(minute);
    Serial.print(":");
    if(second < 10)
      Serial.print("0");
    Serial.print(second);
    Serial.print(", "); 

    Serial.print(temperature); 
    //Serial.print(', ');

    Serial.println();
  }

  digitalWrite(ledPin, HIGH); // turn on the LED, this lets us know our code is running!
  delay(1000);
  
  digitalWrite(ledPin, LOW); // turn off the LED
  delay(1000);
}
