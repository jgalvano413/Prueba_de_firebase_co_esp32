#if defined(ESP32)
#include <WiFi.h>
#include "time.h"
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#define WIFI_SSID "Pixel 7"
#define WIFI_PASSWORD "Galvan69" 
//<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const String JSON = "VENTA: C:0004 V:0025 S:0020 P:0001";
const String path = "/Users/0000F/Maquinas/0003/Logs/Log";
int logNumber = 0;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -21600;
const int   daylightOffset_sec = 0;


void setup()
{
  Serial.begin(115200);
  delay(2000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(3000);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.begin(DATABASE_URL, API_KEY);
  Firebase.setDoubleDigits(5);

}

void loop(){
  logNumber =  logNumber + 1;
  postfirebaseData(JSON + getTime());
  delay(5000);
 
}

void postfirebaseData(String data){
  if (Firebase.setString(fbdo, path + String(logNumber), data)) {
    Serial.println("Valor insertado correctamente en Firebase.");
  } else {
    Serial.println("Error al insertar el valor en Firebase.");
    Serial.println(fbdo.errorReason());
  }
  delay(1000);
  }

String getTime()
{
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return "null";
  }

  char timeBuffer[30];
  strftime(timeBuffer, sizeof(timeBuffer), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  String time = String(timeBuffer);
  return time;  
 }
