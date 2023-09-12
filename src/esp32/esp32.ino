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

const char* serverName = "http://localhost/esp32RemotePhotos/esp_access.php";

const long refreshRate = 5000; // in milliseconds
unsigned long previousMillis = 0; // holds millis value

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
      
      
    } 
  }
}
