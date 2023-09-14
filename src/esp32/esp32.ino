/*
NOTES:
  Millis -  "number of milliseconds that your Arduino board has been powered up"
  millis() - returns that value

*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>

const char* ssid = "A70";
const char* password = "igormaja1";

const char* serverName = "http://192.168.120.1/esp32RemotePhotos/photo_request.php?action=db_check";

const long refreshRate = 5000; // in milliseconds
unsigned long previousMillis = 0; // holds millis value

String requestValue = {};
int LED = 2;

void setup() {
  Serial.begin(115200);
  WifiConnect();
}

void WifiConnect(){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print(" Connection succesfull!! to network with IP Adress: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= refreshRate){
    // check WIFI status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      http.begin(client, serverName);
      int httpResponceCode = http.GET();

      if (httpResponceCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponceCode);
        requestValue = http.getString();
      }else{
        Serial.print("Error code: ");
        Serial.println(httpResponceCode);
      }

      http.end();

      Serial.println(requestValue);
      previousMillis = currentMillis;
    } 
  }
}
