#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

const uint64_t READ_PIPE = 0xF0F0F0F0ABLL;
const uint64_t WRITE_PIPE = 0xF0F0F0F0AALL;

RF24 radio(9, 10);

void setup() {
  Serial.begin(9600);
  Serial.println("Rebooting...");
  Serial.println("Starting radio...");
  radio.begin();
  radio.setRetries(15, 15);
  radio.setAutoAck(true);
  
  radio.openReadingPipe(1, READ_PIPE);
  radio.openWritingPipe(WRITE_PIPE);
  radio.startListening();
  Serial.println("Started listening...");
}

void loop() {

  if (radio.available()) {
    char resp[100];
    
    radio.read(resp, 100);
    Serial.print("Got response: ");
    Serial.println(resp);
  }

  if (Serial.available()) {
    String c = Serial.readString();
        
    radio.stopListening();
    radio.write(c.c_str(), c.length());
    radio.startListening();
    
    Serial.print("You've sent: ");
    Serial.println(c);
  }
}
