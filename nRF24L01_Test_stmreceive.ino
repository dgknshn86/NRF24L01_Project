
#include <Servo.h>
#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

#define KEY1 0
#define KEY2 0

// This is just the way the RF24 library works:
// Hardware configuration: Set up nRF24L01 radio on SPI bus (pins 10, 11, 12, 13) plus pins 7 & 8
RF24 radio(PD2,PD1);

byte addresses[][6] = {"1Node", "2Node"};

int dataRx[]={KEY1,KEY2};
int verilerim[]={KEY1, KEY2};
int servoValue,ledValue;
int values[2];

Servo servoObject;

#define RedLED PD14
#define BlueLED PD15
#define GreenLED PD12
#define OrangeLED PD13

// -----------------------------------------------------------------------------
// SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("THIS IS THE RECEIVER CODE - YOU NEED THE OTHER ARDUINO TO TRANSMIT");
  
  pinMode(BlueLED,OUTPUT);
  pinMode(RedLED,OUTPUT); 
  pinMode(OrangeLED,OUTPUT); 
  pinMode(GreenLED,OUTPUT);  
  servoObject.attach(PA15); 
  
  // Initiate the radio object
  radio.begin();

  // Set the transmit power to lowest available to prevent power supply related issues
  radio.setPALevel(RF24_PA_MIN);

  // Set the speed of the transmission to the quickest available
  radio.setDataRate(RF24_2MBPS);

  // Use a channel unlikely to be used by Wifi, Microwave ovens etc
  radio.setChannel(124);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);

  // Start the radio listening for data
  
  radio.startListening();
}

// -----------------------------------------------------------------------------
// We are LISTENING on this device only (although we do transmit a response)
// -----------------------------------------------------------------------------
void loop() 
{
   // Is there any data for us to get?
      if ( radio.available()) {

      //OrangeLED indicates that radio.available
      digitalWrite(GreenLED,HIGH); 
      digitalWrite(BlueLED,LOW);
      digitalWrite(OrangeLED,LOW);
      
      while(radio.available()){
      radio.read( &dataRx, sizeof(dataRx));

      ledValue=dataRx[0];
      servoValue=dataRx[1];

      
      values[0]=ledValue;
      values[1]=servoValue;

     
      radio.stopListening();
      radio.write( &values, sizeof(values));


      radio.startListening();

     

      
      

      
      }
 
      

   }
      
   
}

/*
      //led
        if((values[0]>=130)){
        digitalWrite(BlueLED,HIGH);
      }
      else if(values[0]<=120){
        
        digitalWrite(OrangeLED,HIGH);   
      }
      else
      {
        digitalWrite(BlueLED,LOW);
        digitalWrite(OrangeLED,LOW);
      }

      //servo
      servoObject.write(servoValue);*/
  

      
