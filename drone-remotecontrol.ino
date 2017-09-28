#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

const uint64_t READ_PIPE = 0xF0F0F0F0ABLL;
const uint64_t WRITE_PIPE = 0xF0F0F0F0AALL;

RF24 radio(9, 10);
char *resp;

void setup() {
  Serial.begin(9600);
  Serial.println("Rebooting...");
  Serial.println("Starting radio...");
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setRetries(15, 15);
  radio.setAutoAck(1);
  radio.openReadingPipe(1, READ_PIPE);
  radio.openWritingPipe(WRITE_PIPE);
  radio.startListening();
  Serial.println("Started listening...");
  resp = new char[32];
  memset(resp, 0, 32);
}

void loop() {

  if (radio.available()) {
    radio.read(resp, 32);

    if (count(resp, ',') == 6) {
      //Serial.print("X: " + getValue(resp, ',', 0));
      //Serial.println(", Y: " + getValue(resp, ',', 1));
      //Serial.println("Altimeter: " + getValue(resp, ',', 3));
      //Serial.println("Temperature: " + getValue(resp, ',', 4));
    } else if (count(resp, ',') == 5) {
      //Serial.println("PID Yaw: " + getValue(resp, ',', 1));
      //Serial.println("PID Roll: " + getValue(resp, ',', 0));
      Serial.println("CW_FRONT: " + getValue(resp, ',', 0));
      Serial.println("CW_BACK: " + getValue(resp, ',', 1));
      Serial.println("CCW_LEFT: " + getValue(resp, ',', 2));
      Serial.println("CCW_RIGHT: " + getValue(resp, ',', 3));
    }
    memset(resp, 0, 32);
  }

  if (Serial.available()) {
    String c = Serial.readString();
        
    radio.stopListening();
    radio.write(c.c_str(), c.length());
    radio.startListening();
    
    Serial.print("Sent: ");
    Serial.println(c);
  }

  delay(50);
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


int count(char* message, char delim) {
  int len = strlen(message);
  int c = 1;

  for (int i = 0; i < len; i++) {
    if (message[i] == delim) {
      c++;
    }
  }

  return c;
}
