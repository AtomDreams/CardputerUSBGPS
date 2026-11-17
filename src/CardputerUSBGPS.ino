#include <Arduino.h>

static const int GPS_RX_PIN = 1;
static const int GPS_TX_PIN = 2;
static const int GPS_BAUD_RATE = 115200;

HardwareSerial GPS_Serial(1);

void setup() {
  Serial.begin(GPS_BAUD_RATE);

  while (!Serial && millis() < 2000) {
    delay(10);
  }

  GPS_Serial.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
}

void loop() {
  while (GPS_Serial.available()) {
    Serial.write(GPS_Serial.read());
  }

  delay(1);
}
