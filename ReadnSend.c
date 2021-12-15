#include <common.h>
#include <Firebase.h>
#include <FirebaseESP32.h>
#include <FirebaseFS.h>
#include <Utils.h>
 #include <WiFi.h>
 #include "DHT.h"
 #define DHTTYPE DHT11
  #define DHTPIN 33
 DHT dht(DHTPIN, DHTTYPE);


#include "addons/TokenHelper.h"

#include "addons/RTDBHelper.h"

#define WIFI_SSID "hidden"
#define WIFI_PASSWORD ""

#define API_KEY "hidden"

#define DATABASE_URL "hidden" 

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;
int soilmoisture =27;
int waterlevel= 2;
int motiontrip= 25;
int fan=26;
int waterpump=12;
int alarming=4;
int moisturevalue;
int alstate=0;
int tankPump=150;
float t=1.0;
float h=1.0;
float hic=1.0;
int Level;
bool abnormalState =false;
bool Pumping;
int Tmax=25;
int Tmin=19;
int Hmax=70;
const int trigPin = 5;
const int echoPin = 17;

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;
bool motiontriggered;
bool fanstate;
void setup() {

pinMode(soilmoisture,INPUT);
pinMode(waterlevel,INPUT);
pinMode(motiontrip,INPUT);
pinMode(DHTPIN,INPUT);
pinMode(fan,OUTPUT);
pinMode(waterpump,OUTPUT);
pinMode(alarming,OUTPUT);
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
dht.begin();
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;


  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; 
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}

void loop() {

moisturesensing();
delay(10);
climate();
delay(10);
//watervelevelsesninsg();
//delay(10);
firebasing();
alert();

}
void firebasing(){
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

  if (Firebase.RTDB.setFloat(&fbdo, "humidity",h)) {
  Serial.println("PASSED3");
  Serial.println("PATH: " + fbdo.dataPath());
  Serial.println("TYPE: " + fbdo.dataType());
}
else {
  Serial.println("FAILED");
  Serial.println("REASON: " + fbdo.errorReason());
}
  if (Firebase.RTDB.setInt(&fbdo, "soil-moisture",moisturevalue)) {
  Serial.println("PASSED");
  Serial.println("PATH: " + fbdo.dataPath());
  Serial.println("TYPE: " + fbdo.dataType());
}
else {
  Serial.println("FAILED");
  Serial.println("REASON: " + fbdo.errorReason());
}
  if (Firebase.RTDB.setFloat(&fbdo, "temperature2",t)) {
  Serial.println("PASSED2");
  Serial.println("PATH: " + fbdo.dataPath());
  Serial.println("TYPE: " + fbdo.dataType());
}
else {
  Serial.println("FAILED");
  Serial.println("REASON: " + fbdo.errorReason());
}
  if (Firebase.RTDB.setFloat(&fbdo, "motion",motiontriggered)) {
  Serial.println("PASSED2");
  Serial.println("PATH: " + fbdo.dataPath());
  Serial.println("TYPE: " + fbdo.dataType());
}
else {
  Serial.println("FAILED");
  Serial.println("REASON: " + fbdo.errorReason());
}
  if (Firebase.RTDB.setFloat(&fbdo, "pump",Pumping)) {
  Serial.println("PASSED2");
  Serial.println("PATH: " + fbdo.dataPath());
  Serial.println("TYPE: " + fbdo.dataType());
}
else {
  Serial.println("FAILED");
  Serial.println("REASON: " + fbdo.errorReason());
}
  if (Firebase.RTDB.setFloat(&fbdo, "fan",fanstate)) {
  Serial.println("PASSED2");
  Serial.println("PATH: " + fbdo.dataPath());
  Serial.println("TYPE: " + fbdo.dataType());
}
else {
  Serial.println("FAILED");
  Serial.println("REASON: " + fbdo.errorReason());
}
    if (Firebase.RTDB.setFloat(&fbdo, "waterlevel",Level)) {
  Serial.println("PASSED2");
  Serial.println("PATH: " + fbdo.dataPath());
  Serial.println("TYPE: " + fbdo.dataType());
}
else {
  Serial.println("FAILED");
  Serial.println("REASON: " + fbdo.errorReason());
   
  }}}
void climate(){
  
  delay(1000);

  h = dht.readHumidity();

  t = dht.readTemperature();

   float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
if(t>Tmax){
  digitalWrite(fan,LOW);
fanstate=true;
abnormalState=true;
  }
else if(t<Tmax){
    digitalWrite(fan,HIGH);
fanstate=false;
abnormalState=true;
}
    
  }
void waterPumpingtoSoil(){
 if (moisturevalue <5){
 digitalWrite(waterpump,LOW);
  
 Pumping=true;}
 else if (moisturevalue>50){
   digitalWrite(waterpump,HIGH);
  
 Pumping=false;}
 else{digitalWrite(waterpump,HIGH);}
 
  }
void moisturesensing(){
 
  moisturevalue= analogRead(soilmoisture);
  //moisturevalue = map(moisturevalue,550,0,0,100);
  Serial.print("Moisture : ");
  Serial.print(moisturevalue);
  Serial.println("%");
  delay(1000);
}
    
void motiondetection(){
  bool isDetected = digitalRead(motiontrip);
 
  if(isDetected){
    Serial.println("Presence detected");
    bool motiontriggered=1;
  }
 
  delay(500);
  }
void watervelevelsesninsg(){
  Level = analogRead(waterlevel);

  Serial.print("The water sensor value: ");
  Serial.println(Level);
  delay(1000);
}
    
    
   
  
  
void alert(){
  if (abnormalState){
 digitalWrite(alarming,HIGH);}
 else    
  digitalWrite(alarming,LOW);
    
    }
  
  
  
