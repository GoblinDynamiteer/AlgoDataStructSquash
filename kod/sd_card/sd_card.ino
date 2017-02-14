/*
  SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

File logfile;
#define CS_PIN 10
int rad = 1;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(CS_PIN)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void loop() {
  SD.remove("log.txt");
  logfile = SD.open("log.txt", FILE_WRITE);
  logfile.println("----------------------");
  for(int i = 0; i< 20; i++){
    logfile.print("Rad ");
    logfile.print(rad++);
    logfile.println(" testText");
  }
  logfile.println("----------------------");
  logfile.close();
  logfile = SD.open("log.txt");
  if(logfile){
    Serial.println("log.txt:");
    // read from the file until there's nothing else in it:
    while (logfile.available()) {
      Serial.write(logfile.read());
    }
    //close the file:
    logfile.close();
  }
  else{
    // if the file didn't open, print an error:
    Serial.println("error opening log.txt");
  }
  delay(1000);
}
