/*
  SD card datalogger

 This example shows how to log data from three analog sensors
 to an SD card using the SD library.

 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 *  ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 * MEGA
 ** MOSI - pin 51
 ** MISO - pin 50
 ** CLK - pin 52
 ** CS - pin 53

 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

#include "DHT.h"
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT SA(DHTPIN, DHTTYPE);//Sensor A
const int chipSelect = 53;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   SA.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  pinMode(13,OUTPUT);
}

bool st = false;

//VARIABLES PARA LOS SENSORES DE HUMEDAD
float hA = 0.0; float hB = 0.0; //humedad 
float tA = 0.0; float tB = 0.0; //temperatura


//long ms = millis();

void loop() {
  st = !st;
  digitalWrite(13,st);

  
  LeerSensorA();
  delay(2000);
  //digitalWrite(13,true);
  // make a string for assembling the data to log:
  String dataString = "";

  // read three sensors and append to the string:
  
  dataString += String(tA) + "; " + String(hA) + "; " +  String(millis());  
  //ms = millis();
//  for (int analogPin = 0; analogPin < 3; analogPin++) {
//    int sensor = analogRead(analogPin);
//    dataString += String(sensor);
//    if (analogPin < 2) {
//      dataString += ",";
//    }
//  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
    ReSD();
    
  }
  
}



void LeerSensorA(){
  tA = SA.readTemperature();hA = SA.readHumidity();
  //tA = tA + 1; 
  hA = hA + (analogRead(0) / 10);//esto es solo un ofset
  if (hA > 99.9){hA = 99.9;}
}



void ReSD(){
  Serial.println("Re intentando SD");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  }



