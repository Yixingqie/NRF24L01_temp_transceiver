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

//include the below libraries
#include <SPI.h>   
#include "RF24.h"  
#include "printf.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> 

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define  CE_PIN  7   // The pins to be used for CE and SN
#define  CSN_PIN 8


//sets up reciever
RF24 radio(CE_PIN, CSN_PIN);

//sets up connection pipes
byte addresses[][6] = {"test", "2Node"}; 

//struct package
struct dataStruct {
  unsigned long _micros;  
  int temp;          
} myData;                 


void setup()   
{
  //display intializations
  display.clearDisplay();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  display.clearDisplay();
  Serial.begin(115200);  
  printf_begin(); 

  //nRFL01 Radio intilization
  radio.begin();          
  radio.setChannel(108);  
  radio.setDataRate(RF24_250KBPS); // set speed
  radio.setPALevel(RF24_PA_MAX);//max for most range--> high power usuage

  //opens pipes
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  // Start the radio listening for data
  radio.startListening();

}

void loop()  
{
  //sets the text color
  display.setTextSize(8.5);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  //while the radio is available -- > keep listening
  if ( radio.available())
  {
    while (radio.available())   
    {
      radio.read( &myData, sizeof(myData) );             // Get the data
    }

    radio.stopListening();                               
    radio.write( &myData, sizeof(myData) );              // Sends confirmation
    radio.startListening();                              

    //prints out the temp data to serial monitor
    Serial.print(myData.temp);
    display.println((int)((((myData.temp *5.0)/1024.0)-0.5)*100)); //conversion to celsius
    display.display();
    display.clearDisplay(); //refreshes display
  } 

display.clearDisplay();
}
