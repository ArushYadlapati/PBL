#include <Arduino.h>
#include "DHT_Async.h"
#define DHT_SENSOR_TYPE DHT_TYPE_11
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensorValue;
int digitalValue;
int airCount = 0;
int buttonPressed = 0;
int sensorTrigger = 350;

static const int DHT_SENSOR_PIN = 3;
DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void setup() {
    pinMode(2, INPUT);
    pinMode(3, OUTPUT);
    pinMode(4, INPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(10, INPUT_PULLUP);
    Serial.begin(9600);
    
    // initialize the LCD
    lcd.begin();

    // Turn on the blacklight
    lcd.setBacklight((uint8_t)1);
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
    
    sensorValue = analogRead(0);
    digitalValue = digitalRead(2);
    
    Serial.println(buttonPressed);
    Serial.println(sensorValue);
    Serial.println(airCount);

    delay(100);

    if ((airCount > 20) and (sensorValue > sensorTrigger) and (buttonPressed == 1)) {
        if (buttonPressed == 1) {      
            Serial.println(sensorValue);
            Serial.print(buttonPressed);
            tone(9, 32);
            delay(1000);
            noTone(9);
          }
    }

    if ((sensorValue > sensorTrigger) and airCount < 50) {
        airCount ++;  
    }
    
    else {
      noTone(9);
      if ((sensorValue < 250) and (airCount > 0)) {
          airCount = airCount - 1;
        
      }
    }
    
    if (measure_environment(&temperature, &humidity)) {
        if (digitalRead(10) == LOW) {
          if (buttonPressed == 0) {
            buttonPressed = 1;    
          }
          else {
            buttonPressed = 0;
        }
          delay(200);
        }
      
        Serial.print("T = ");
        Serial.print(temperature, 1);
        Serial.print(" deg. C, H = ");
        Serial.print(humidity, 1);
        Serial.println("%");
        
        String finalTemp = (String(temperature), " C");

        lcd.clear();

        lcd.setCursor(0, 0);
        lcd.print(" Temp    Air Q");
        
        lcd.setCursor(0, 1);
        lcd.print(temperature - 3.5);
        lcd.print("C");
        lcd.print(" ");


        if (sensorValue < 200) {
          lcd.print(sensorValue);
          lcd.print(": Good");  
        }

        else if (sensorValue < 250) {
          lcd.print(" ");
          lcd.print(sensorValue);
          lcd.print(": Ok");  
        }

        else if (sensorValue < 275) {
          lcd.print(sensorValue);
          lcd.print(": Poor");  
        }

        else {
          lcd.print(sensorValue);
          lcd.print(": Bad");  
        }
        
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
        
        if (digitalRead(10) == LOW){
          if (buttonPressed == 0){
            buttonPressed = 1;    
          }
          else {
            buttonPressed = 0;
        }
          delay(200);
        }
    }
}
