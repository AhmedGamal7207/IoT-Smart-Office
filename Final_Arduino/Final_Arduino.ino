#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include "DFRobot_AHT20.h" // Temperature Sesnor

DFRobot_AHT20 aht20; // Our Temperature Sensor



void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  aht20.begin(); // Temperature Sensor

  Serial.begin(115200); // Set the baud rate to match ESP32

}

void loop() {
  float tempValue = getTemperature();

  float humValue = getHumidity();

  lcd.clear();

  // Set the cursor to the beginning of the first row
  lcd.setCursor(0, 0);
  // Print the temperature value
  lcd.print("Temp: ");
  lcd.print(tempValue);
  lcd.print(" C");

  // Set the cursor to the beginning of the second row
  lcd.setCursor(0, 1);
  // Print the humidity value
  lcd.print("Hum : ");
  lcd.print(humValue);
  lcd.print(" %");

  delay(500);


}

float getTemperature(){
  if(aht20.startMeasurementReady(true)){
    float temp = aht20.getTemperature_C();
    return temp;
  }
}

float getHumidity(){
  if(aht20.startMeasurementReady(true)){
    float hum = aht20.getHumidity_RH();
    return hum;
  }
}




