/* ***************************************************BLYNK CONFIGURATION*************************************************** */
#define BLYNK_TEMPLATE_ID "TMPL2M2IC1pD8"       // Blynk Template ID
#define BLYNK_TEMPLATE_NAME "test1"             // Blynk Template Name
#define BLYNK_AUTH_TOKEN "SF5KioXxyjOkTIpL3KguOSfZAIYkTjd7"  // Blynk Auth Token

/* ***************************************************PACKAGES*************************************************** */
#include <ESP32Servo.h>  // Servo Motor
#include <WiFi.h>        // Wi-Fi
#include <BlynkSimpleEsp32.h> // Blynk for ESP32
#include <NTPClient.h>   // NTP server to get real date and time
#include <WiFiUdp.h>     // Wi-Fi UDP for NTP server
#include "time.h"        // Time package

/* ***************************************************DATE AND TIME*************************************************** */
const char* ntpServer = "pool.ntp.org"; // NTP server
const long gmtOffset_sec = 3 * 3600;    // GMT+3 (Egypt)
const int daylightOffset_sec = 3600;   // Summer Time

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec);

/* ***************************************************SERVO*************************************************** */
Servo myServo; 
int servoPin = 4;

/* ***************************************************SENSORS*************************************************** */
const int flamePin = 14; 
const int forcePin = 32;  
const int ldrPin = 15; 
const int irPin = 18; 
const int rainPin = 5; 
const int motionPin = 13;

/* ***************************************************ACTUATORS*************************************************** */
const int ledMotionPin = 2; 
const int ledFlamePin = 23;
const int ledLdrPin = 27;

const int buzzerRainPin = 12;

const int motor1Pin2 = 19;
const int motor1Pin1 = 22;
const int enablePin1 = 21;

/* ***************************************************WIFI*************************************************** */
const char* ssid = "KAG"; 
const char* password = "block##5728069";

void setup() {
  Serial.begin(115200);

  /* ***************************************************INPUT PINS (Sensors)*************************************************** */
  pinMode(flamePin, INPUT);
  pinMode(forcePin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(irPin, INPUT);
  pinMode(rainPin, INPUT);
  pinMode(motionPin, INPUT);

  /* ***************************************************OUTPUT PINS (Actuators)*************************************************** */
  pinMode(ledMotionPin, OUTPUT);
  pinMode(ledFlamePin, OUTPUT);
  pinMode(ledLdrPin, OUTPUT);
  pinMode(buzzerRainPin, OUTPUT);
  myServo.attach(servoPin);
  myServo.write(70);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  
  analogWrite(enablePin1, 255);

  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  /* ***************************************************BLYNK INIT*************************************************** */
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  /* ***************************************************NTP INIT*************************************************** */
  timeClient.begin();
  timeClient.update();

  Serial.println("Setup Was Ended Successfully!");
}

void loop() {
  Blynk.run(); // Run Blynk
  timeClient.update();

  // Update and send sensor data to Blynk
  Blynk.virtualWrite(V0, getFlame());
  Blynk.virtualWrite(V1, getForce());
  Blynk.virtualWrite(V2, getLdr());
  Blynk.virtualWrite(V3, getIr());
  Blynk.virtualWrite(V4, getRain());
  Blynk.virtualWrite(V5, getMotion());
}

/* ***************************************************ACTUATORS CONTROL FROM BLYNK*************************************************** */
// Control LED for flame
BLYNK_WRITE(V6) {
  int flameLEDState = param.asInt(); // Value from Blynk app (0 or 1)
  digitalWrite(ledFlamePin, flameLEDState);
}

// Control servo motor
BLYNK_WRITE(V7) {
  int servoPosition = param.asInt(); // Value from Blynk slider (0-180)
  myServo.write(servoPosition);
}

// Control motor
BLYNK_WRITE(V8) {
  int motorState = param.asInt(); // Value from Blynk app (0 or 1)
  if (motorState == 1) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
  } else {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
  }
}

/* ***************************************************SENSOR FUNCTIONS*************************************************** */
int getFlame() {
  return digitalRead(flamePin);
}

int getForce() {
  return analogRead(forcePin);
}

int getLdr() {
  return digitalRead(ldrPin);
}

int getIr() {
  return digitalRead(irPin);
}

int getRain() {
  return digitalRead(rainPin);
}

int getMotion() {
  return digitalRead(motionPin);
}

void print_all_readings(){
  int flameValue = getFlame();

  int forceValue = getForce();

  int ldrValue = getLdr();

  int irValue = getIr();

  int rainValue = getRain();

  int motionValue = getMotion();

  Serial.print("Flame: ");
  Serial.println(flameValue);

  Serial.print("Force: ");
  Serial.println(forceValue);

  Serial.print("LDR: ");
  Serial.println(ldrValue);

  Serial.print("IR: ");
  Serial.println(irValue);

  Serial.print("Rain: ");
  Serial.println(rainValue);

  Serial.print("Motion: ");
  Serial.println(motionValue);

  Serial.println("-------------------");

  delay(400);
}

