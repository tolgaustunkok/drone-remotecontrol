#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

const uint64_t READ_PIPE = 0xF0F0F0F0ABLL;
const uint64_t WRITE_PIPE = 0xF0F0F0F0AALL;

typedef struct {
  float x;
  float y;
  float z;
} sensor_data_t;

typedef struct {
  sensor_data_t sensorData;
  float altitude;
  float temperature;
  int motorThrusts[4];
  float pidRoll;
  float pidPitch;
} debug_data_t;

typedef enum {
  CW_FRONT = 0,
  CW_BACK = 1,
  CCW_LEFT = 2,
  CCW_RIGHT = 3
} MOTOR_E;

RF24 radio(9, 10);
debug_data_t resp;

void setup() {
  Serial.begin(9600);
  Serial.println("Rebooting...");
  Serial.println("Starting radio...");
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  //radio.setDataRate(RF24_250KBPS);
  radio.setRetries(15, 15);
  //radio.setAutoAck(0);
  radio.openReadingPipe(1, READ_PIPE);
  radio.openWritingPipe(WRITE_PIPE);
  radio.startListening();
  Serial.println("Started listening...");
}

void loop() {

  if (radio.available()) {
    radio.read(&resp, sizeof(debug_data_t));

    Serial.println("Inclination: " + String(resp.sensorData.x) + "," + String(resp.sensorData.y) + "," + String(resp.sensorData.z));
    Serial.println("Altitude: " + String(resp.altitude));
    Serial.println("Temperature: " + String(resp.temperature));
    Serial.print("Motor Thrusts: "); // Clockwise order starting with CW_FRONT
    Serial.println(String(resp.motorThrusts[CW_FRONT]) + "," + String(resp.motorThrusts[CCW_RIGHT]) + "," + String(resp.motorThrusts[CW_BACK]) + "," + String(resp.motorThrusts[CCW_LEFT]));
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
