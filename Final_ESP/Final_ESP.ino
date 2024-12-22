/* ***************************************************PACKAGES*************************************************** */
//#include "DFRobot_AHT20.h" // Temperature Sesnor
#include <ESP32Servo.h>  // Servo Motor
#include <WiFi.h> // Wifi
#include <Firebase_ESP_Client.h> // Firebase
#include <addons/TokenHelper.h> // Firebase Feedback Messages (Token Management)
#include <addons/RTDBHelper.h> // Firebase Real Time Data Base
#include <NTPClient.h> // NTP server to get the real date and time
#include <WiFiUdp.h> // Wifi library to access NTP server
#include "time.h" // Time package


/* ***************************************************DATE AND TIME*************************************************** */
const char* ntpServer = "pool.ntp.org"; // NTP server to use
const long gmtOffset_sec = 3 * 3600;  // 3 because of GMT+3 (Egypt) "3600 seconds = 1 hour"
const int daylightOffset_sec = 3600;  // Summer Time

WiFiUDP ntpUDP; // Wifi client for ntp server
NTPClient timeClient(ntpUDP, ntpServer, gmtOffset_sec, daylightOffset_sec); // This is the NTP Client Object
String dateNow = "";

/* ***************************************************SERVO*************************************************** */
Servo myServo; // Our Servo Motor
int servoPin = 4 ; // Servo Motor Pin

/* ***************************************************SENSORS*************************************************** */
const int flamePin = 14; // Flame Sensor Pin (Inverted)
const int forcePin = 32;  // Force Sensor Pin (Analog Reading)
const int ldrPin = 15; // LDR pin (Not Inverted)
const int irPin = 18; // IR Sensor Pin (Inverted)
const int rainPin=5; // Rain Sensor Pin (Inverted)
const int motionPin = 13; // Motion Sensor Pin (Not Inverted)

/* ***************************************************Actuators*************************************************** */
const int ledMotionPin = 2; // Led Pin //here
const int ledFlamePin = 23; // Led Pin
const int ledLdrPin = 27; // Led Pin

const int buzzerRainPin = 12 ; // Buzzer Pin

const int motor1Pin2 = 19; // IN1 Pin of L298N for Motor 2 (Different pin)
const int motor1Pin1 = 22; // IN2 Pin of L298N for Motor 2 (Different pin)
const int enablePin1 = 21;  // ENA Pin of L298N for Motor 1

/* ***************************************************WIFI*************************************************** */
const char* ssid = "R00"; // Connect to this network
const char* password = "password";

/* ***************************************************FIREBASE*************************************************** */
String USER_EMAIL = "SmartOffice123@gmail.com";
String USER_PASSWORD = "iot123456789";
String userID = ""; // To store UserID after signing in

FirebaseData fbdo; // Firebase Data Object (Used to convert data From and To JSON before sending and recieving from firebase)
FirebaseAuth auth; // Firebase Auth Object (Used to handle Authentication(Email and Password))
FirebaseConfig config; // Firebase Config Object (Used to set the Settings of Firebase (For EX: API Key))

#define API_KEY "AIzaSyDmDkkTMXtJrwtZn1BnrPP2-P8XhMfZrm0" // API Key for Elderly Care Project
#define DATABASE_URL "https://iot-smart-office-5e623-default-rtdb.firebaseio.com/" // RTDB Link from Firebase

String espPath = ""; // String to store path of ESP data in RTDB Firebase
String fullPath = ""; // Store the path with Date and Time
String currentPath = ""; // Store the path with Date and Current

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Beginning the Serial Communication and set the Baud Rate (Speed of Connection) = 115200

  /* ***************************************************INPUT PINS (Sensors)*************************************************** */
  pinMode(flamePin, INPUT);
  pinMode(forcePin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(irPin, INPUT);
  pinMode(rainPin, INPUT);
  pinMode(motionPin, INPUT);

  /* ***************************************************OUTPUT PINS (Actuators)*************************************************** */
  pinMode(ledMotionPin,OUTPUT);
  pinMode(ledFlamePin,OUTPUT);
  pinMode(ledLdrPin,OUTPUT);

  pinMode(buzzerRainPin,OUTPUT);

  myServo.attach(servoPin);
  myServo.write(70);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  
  analogWrite(enablePin1, 255); // Enable Motor driver

  // Initially stop both motors
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  /* ***************************************************FUNCTIONS CALLS*************************************************** */
  delay(100);
  connectToWifi(ssid,password);
  startFirebase();
  startDateTime();
  dateNow = getDate();
  Serial.println("Setup Was Ended Successfully!");

}

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update(); // Update the time from the NTP server
  String timeNow = timeClient.getFormattedTime();
  if(Firebase.ready()){
    fullPath = espPath+dateNow+"/"+timeNow+"/"; // Update full path with current Date and Time
    setStringFB(currentPath+"timeNow/",timeNow); // Store time in the current data

    int flameValue = getFlame();

    int forceValue = getForce();

    int ldrValue = getLdr();

    int irValue = getIr();

    int rainValue = getRain();

    int motionValue = getMotion();

    if(flameValue == 0){
      digitalWrite(ledFlamePin,HIGH);
    }
    else{
      digitalWrite(ledFlamePin,LOW);
    }
    if(forceValue >= 500){
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      delay(1000);
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
    
    }
    else{
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
    }
    if(ldrValue == 0){
      digitalWrite(ledLdrPin,HIGH);
    }
    else{
      digitalWrite(ledLdrPin,LOW);
    }
    if(irValue == 0){
      myServo.write(0);
      delay(1000);
      myServo.write(70);
    }
    else{
      myServo.write(70);
    }
    if(rainValue == 0){
      digitalWrite(buzzerRainPin, HIGH);
      delay(500);
      digitalWrite(buzzerRainPin, LOW);
      delay(500);
      digitalWrite(buzzerRainPin, HIGH);
      delay(500);
      digitalWrite(buzzerRainPin, LOW);
      delay(500);
    }
    else{
      digitalWrite(buzzerRainPin, LOW);
    }
    if(motionValue == 1){
      digitalWrite(ledMotionPin, HIGH);
    }
    else{
      digitalWrite(ledMotionPin, LOW);
    }
  }
  
}

/* ***************************************************Wifi Functions*************************************************** */
// Function to manage connecting to Wifi
void connectToWifi(const char* ssid, const char* password){ 
  WiFi.begin(ssid, password); // Start connection to WIFI

  Serial.print("Connecting to "); 
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) { // While connecting (not connected)
    delay(1000);
    Serial.print("."); // Print dot every second (Connecting .....)
  }

  // After being connected successfully
  Serial.println("\nConnected to WiFi"); // Print connected
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print IP
  
}

/* ***************************************************Firebase Functions*************************************************** */
// Function to manage initializing the Firebase
void startFirebase(){
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION); // Print the Version of Firebase

  config.api_key = API_KEY; // Set the API Key of the Project

  /* Authentication */
  auth.user.email = USER_EMAIL; // Set the User Email
  auth.user.password = USER_PASSWORD; // Set the User Password

  /* Configuration */
  Firebase.reconnectWiFi(true); // If WIFI is disconnected at any time, reconnect.
  fbdo.setResponseSize(4096); // Sets the size (in bytes) of the transferred data (between ESP and Firebase)

  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // To manage Firebase Authentication Tokens

  Firebase.begin(&config, &auth);
  delay(5000);

  if(Firebase.ready()){ // If Firebase Started
    Serial.println("Firebase Authenticated Successfully");
    userID = auth.token.uid.c_str(); // Get the user's unique ID (UID)
    espPath = "ESP/"+userID+"/";
    currentPath = espPath+"current"+"/";
  }
  else{
    Serial.println("Firebase Could Not Authenticate");
    startFirebase();
  }
}


// Function to set int value on Firebase
bool setIntFB(String path, int value){
  if (Firebase.RTDB.setInt(&fbdo, path, value)){
        /*Serial.println("Data Inserted Successfully");
        Serial.println("PATH: " + fbdo.dataPath());*/
        return true;
      }
      else {
        Serial.println("Failed to send Value: "+String(value)+" to Path: "+path);
        Serial.println("REASON: " + fbdo.errorReason());
        delay(1000);
        return false;
      }
}

// Function to set int value from Firebase
int getIntFB(String path){
  if (Firebase.RTDB.get(&fbdo, path)) {
    if (fbdo.dataType() == "int") { // Check if the returned type is int
      int returned_value = fbdo.intData();
      return returned_value;
    }
  } 
  else {
    Serial.println("Error getting data from Path: "+path);
    Serial.println("Reason: " + fbdo.errorReason());
    delay(1000);
    return 0;
  }
}

// Function to set string value on Firebase
bool setStringFB(String path, String value){
  if (Firebase.RTDB.setString(&fbdo, path, value)){
    /*Serial.println("Data Inserted Successfully");
    Serial.println("PATH: " + fbdo.dataPath());*/
    return true;
  }
  else {
    Serial.println("Failed to send Value: " + value + " to Path: " + path);
    Serial.println("REASON: " + fbdo.errorReason());
    delay(1000);
    return false;
  }
}

// Function to get string value from Firebase
String getStringFB(String path){
  if (Firebase.RTDB.get(&fbdo, path)) {
    if (fbdo.dataType() == "string") { // Check if the returned type is string
      String stringValue_FB = fbdo.stringData();

      /*Serial.print("String Value from Firebase: ");
      Serial.println(stringValue_FB);*/
      return stringValue_FB;
    }
  } 
  else {
    Serial.println("Error getting data from Firebase");
    Serial.println("Reason: " + fbdo.errorReason());
    return ""; // Return an empty string or handle the error as needed
  }
}

bool setFloatFB(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path, value)){
    /*Serial.println("Data Inserted Successfully");
    Serial.println("PATH: " + fbdo.dataPath());*/
    return true;
  }
  else {
    Serial.println("Failed to send Value: " + String(value) + " to Path: " + path);
    Serial.println("REASON: " + fbdo.errorReason());
    delay(1000);
    return false;
  }
}

// Function to get float value from Firebase
float getFloatFB(String path){
  if (Firebase.RTDB.getFloat(&fbdo, path)) {
    if (fbdo.dataType() == "float") {
      float floatValue_FB = fbdo.floatData();

      /*Serial.print("Float Value from Firebase: ");
      Serial.println(floatValue_FB);*/
      return floatValue_FB;
    }
  } 
  else {
    Serial.println("Error getting data from Firebase");
    Serial.println("Reason: " + fbdo.errorReason());
    return 0.0; // Return a default value or handle the error as needed
  }
}


/* ***************************************************DATE AND TIME FUNCTIONS*************************************************** */
// Function to start date and time
void startDateTime(){
  timeClient.begin();
  timeClient.update();
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

// Function to get the data dd\mm\yyyy
String getDate(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return "date";
  }

  char dayNumBuffer[20];
  strftime(dayNumBuffer, sizeof(dayNumBuffer), "%d", &timeinfo); // Extract day in string format
  String dayNum = String(dayNumBuffer);

  char monthBuffer[20];
  strftime(monthBuffer, sizeof(monthBuffer), "%B", &timeinfo); // Extract month name in string format
  String month = String(monthBuffer);
  String monthNum = String(getMonthNumber(month)); // convert from month name to month num

  char yearBuffer[20];
  strftime(yearBuffer, sizeof(yearBuffer), "%Y", &timeinfo); // Extract year number in string format
  String year = String(yearBuffer);

  String date = dayNum+"\\"+monthNum+"\\"+year; // dd\mm\yyyy
  return date;
}

String months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
// Function to convert from Month Name to Month Number
int getMonthNumber(String target) {
  for (int i = 0; i < 12; i++) {
    if (months[i] == target) {
      return i+1; // Return the index when the target element is found + 1 (Januare index 0 + 1 = 1 "Month Num")
    }
  }
  return -1; // Return -1 if the element is not found in the array
}

/* ***************************************************SENSORS FUNCTIONS*************************************************** */

int getFlame(){
  int flame = digitalRead(flamePin);
  setIntFB(currentPath+"flameSensor",flame);
  return flame;
}

int getForce(){
  int force = analogRead(forcePin);
  setIntFB(currentPath+"forceSensor",force);
  return force;
}

int getLdr(){
  int ldr = digitalRead(ldrPin); // Reading the value of LDR
  setIntFB(currentPath+"ldrSensor",ldr);
  return ldr;
}

int getIr(){
  int ir = digitalRead(irPin);
  setIntFB(currentPath+"irSensor",ir);
  return ir;
}

int getRain(){
  int rain = digitalRead(rainPin);
  setIntFB(currentPath+"rainSensor",rain);
  return rain;
}

int getMotion(){
  int motion = digitalRead(motionPin);
  setIntFB(currentPath+"motionSensor",motion);
  return motion;
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

/* ***************************************************ACTUATORS FUNCTIONS*************************************************** */

void test_all_actuators() {
  // Test LED for motion
  digitalWrite(ledMotionPin, HIGH);
  delay(500);
  digitalWrite(ledMotionPin, LOW);
  delay(500);

  // Test LED for flame
  digitalWrite(ledFlamePin, HIGH);
  delay(500);
  digitalWrite(ledFlamePin, LOW);
  delay(500);

  // Test LED for LDR
  digitalWrite(ledLdrPin, HIGH);
  delay(500);
  digitalWrite(ledLdrPin, LOW);
  delay(500);

  // Test buzzer for rain
  digitalWrite(buzzerRainPin, HIGH);
  delay(500);
  digitalWrite(buzzerRainPin, LOW);
  delay(500);

  // Test motor 1
  digitalWrite(motor1Pin1, HIGH);
  delay(500);
  digitalWrite(motor1Pin2, LOW);
  delay(500);

  // Test motor 2
  digitalWrite(motor1Pin2, HIGH);
  delay(500);
  digitalWrite(motor1Pin1, LOW);
  delay(500);

  Serial.println("Output devices test complete.");
}