#include <Arduino.h>
#include "DHT_Async.h"
#define DHT_SENSOR_TYPE DHT_TYPE_11

int sensorValue;
int digitalValue;
int airCount = 0;
int buttonPressed = 0;

static const int DHT_SENSOR_PIN = 2;
DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void setup() {
    pinMode(3, OUTPUT);
    pinMode(4, INPUT);
    pinMode(10, INPUT_PULLUP);
    Serial.begin(115200);
}

static bool measure_environment(float *temperature, float *humidity) {
    static unsigned long measurement_timestamp = millis();
    if (millis() - measurement_timestamp > 3000ul) {
        if (dht_sensor.measure(temperature, humidity)) {
            measurement_timestamp = millis();
            return (true);
        }
    }
    return (false);
}

void loop() {
    if (digitalRead(10) == LOW){
      if (buttonPressed == 0){
        buttonPressed = 1;    
      }
      else{
        buttonPressed = 0;
      }
      delay(200);
    }
    
    float temperature;
    float humidity;

    delay(100);
    
    sensorValue = analogRead(0);
    digitalValue = digitalRead(2);
    
    Serial.println(airCount);

    if (airCount > 20) {
        if (sensorValue > 50) {
          if (buttonPressed == 1){
            if (digitalRead(10) == HIGH) {
              Serial.println(sensorValue);
              Serial.print(" ");
              Serial.print(buttonPressed);
              tone(3, 32);
              delay(1000);
              noTone(3);
            }
          }
        }

    }

    if ((sensorValue > 50) and airCount < 50) {
        airCount ++;  
    }
    
    else {
      noTone(3);
      Serial.println(sensorValue);
      Serial.print(" ");
      Serial.print(buttonPressed);
      if (sensorValue < 175) {
        if (airCount > 0) {
          airCount = airCount - 1;
        }
      }
    }
    
    if (measure_environment(&temperature, &humidity)) {
        Serial.print("T = ");
        Serial.print(temperature, 1);
        Serial.print(" deg. C, H = ");
        Serial.print(humidity, 1);
        Serial.println("%");
        if (temperature > 35) {
          tone(3, 554);
          delay(100);
          noTone(3);
        }

        if (temperature < 20) {
          tone(3, 354);
          delay(100);
          noTone(3);
        }
        
        else {
          noTone(3);
        }  
    }
    
}
