/*Connections for the NRF24L01
   1 - GND
   2 - VCC 3.3V 
   3 - CE to Arduino pin 7
   4 - CSN to Arduino pin 8
   5 - SCK to Arduino pin 13
   6 - MOSI to Arduino pin 11
   7 - MISO to Arduino pin 12
   8 - UNUSED
*/

//include the following libraries
#include <SPI.h>   
#include "RF24.h"  
#include "printf.h" 

#define  CE_PIN  7   // The pins to be used for CE and SN
#define  CSN_PIN 8

#define input   A0  // input from thermistor

//hardware configuring
RF24 radio(CE_PIN, CSN_PIN);

//set up connection pipes
byte addresses[][6] = {"test", "2Node"}; 

struct dataStruct {
  unsigned long _micros;  // to save response times
  int temp;          //temp data to be transmitted
} myData;                 


void setup()   
{
  Serial.begin(115200);  //sets up serial monitor
  radio.begin();          // Initialize the nRF24L01 Radio
  radio.setChannel(108);  
  radio.setDataRate(RF24_250KBPS); // speed

  radio.setPALevel(RF24_PA_MAX);//max range-->power usuage:high

  // opens the pipes for connections
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);


void loop()   
{
  radio.stopListening();                                   //stops listening
  myData.temp = analogRead(input);
  Serial.println(myData.temp);
  myData._micros = micros();  // Send back for timing

  if (!radio.write( &myData, sizeof(myData) )) {           //continous data sending to prevent lag
    Serial.println(F("Transmit failed "));
  }
  delay(1000);

}
