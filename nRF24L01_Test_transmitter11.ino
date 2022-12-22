
#include "Arduino.h"
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <Servo.h>

#define KEY1 0
#define KEY2 0

#define yeksen A1
#define xeksen A0
#define led 2

Servo servoObject;

// This is just the way the RF24 library works:
// Hardware configuration: Set up nRF24L01 radio on SPI bus (pins 10, 11, 12, 13) plus pins 7 & 8
RF24 radio(9, 10);

int a,b;
int verilerim [2];
int dataRx[]={KEY1,KEY2};


byte addresses[][6] = {"1Node", "2Node"};

// -----------------------------------------------------------------------------
// SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP   SETUP
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("THIS IS THE TRANSMITTER CODE - YOU NEED THE OTHER ARDIUNO TO SEND BACK A RESPONSE");

  pinMode(led,OUTPUT);
  pinMode(yeksen, INPUT);
  pinMode(xeksen, INPUT);
  servoObject.attach(4);

  
  // Initiate the radio object
  radio.begin();

  // Set the transmit power to lowest available to prevent power supply related issues
  radio.setPALevel(RF24_PA_MIN);

  // Set the speed of the transmission to the quickest available
  radio.setDataRate(RF24_2MBPS);

  // Use a channel unlikely to be used by Wifi, Microwave ovens etc
  radio.setChannel(124);

  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

}

// -----------------------------------------------------------------------------
// LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP
// -----------------------------------------------------------------------------
void loop() {

  // Ensure we have stopped listening (even if we're not) or we won't be able to transmit
  radio.stopListening(); 

  a=analogRead(A0);
  b=analogRead(A1);

  verilerim[0]=a;
  verilerim[1]=b;
  
   if (!radio.write( &verilerim, sizeof(verilerim) )) {
    Serial.println("No acknowledgement of transmission - receiving radio device connected?");    
  }
  // Now listen for a response
  radio.startListening();
   // But we won't listen for long, 200 milliseconds is enough
  unsigned long started_waiting_at = millis();
  // Loop here until we get indication that some data is ready for us to read (or we time out)
  while ( ! radio.available() ) {
    // Oh dear, no response received within our timescale
    if (millis() - started_waiting_at > 200 ) {
      Serial.println("No response received - timeout!");
      return;
    }
  }
  radio.read(&dataRx,sizeof(dataRx));

    Serial.print(" received ledVal:");
    Serial.print(dataRx[0]);
    Serial.print(" ,received servoVal:");
    Serial.println(dataRx[1]);
  

}
