#include <Arduino.h>
#include <WiFiManager.h>
#include <fauxmoESP.h>

fauxmoESP fauxmo;

#define SERIAL_BAUDRATE 115200
#define PIN1_PIN 3
#define PIN2_PIN 2
#define PIN3_PIN 0
#define PIN1_NAME "pin 1"
#define PIN2_NAME "pin 2"
#define PIN3_NAME "pin 3"

void setup() {
  // set RX pin to be GPIO
  pinMode(3, FUNCTION_3);

  pinMode(PIN1_PIN, OUTPUT);
  pinMode(PIN2_PIN, OUTPUT);
  pinMode(PIN3_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);

  WiFiManager wm;

//  wm.resetSettings();

  bool res;

  res = wm.autoConnect("AlexaBlock", "12345678");

  if(!res) {
    Serial.println("Failed to connect.");
  } else {
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices
    fauxmo.enable(true);

    fauxmo.addDevice(PIN1_NAME);
    fauxmo.addDevice(PIN2_NAME);
    fauxmo.addDevice(PIN3_NAME);

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
      Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
      if(strcmp(device_name, PIN1_NAME)==0) {
        if(state) {
          analogWrite(PIN1_PIN, value);
        } else {
          digitalWrite(PIN1_PIN, LOW);
        }
      } else if(strcmp(device_name, PIN2_NAME)==0) {
        if(state) {
          analogWrite(PIN2_PIN, value);
        } else {
          digitalWrite(PIN2_PIN, LOW);
        }
      } else if(strcmp(device_name, PIN3_NAME)==0) {
        if(state) {
          analogWrite(PIN3_PIN, value);
        } else {
          digitalWrite(PIN3_PIN, LOW);
        }
      }
    });
  }
}

void loop() {
  fauxmo.handle();

  // This is a sample code to output free heap every 5 seconds
  // This is a cheap way to detect memory leaks
  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }

}
