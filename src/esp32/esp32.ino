/*
NOTES:
  Millis -  "number of milliseconds that your Arduino board has been powered up"
  millis() - returns that value

*/
#include "esp_camera.h"

#define CAMERA_MODEL_AI_THINKER // camera model (check camera_pins.h to see all suported models)
#include "camera_pins.h" // pin definitions here

#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

const char* ssid = "A70";
const char* password = "igormaja1";

const char* serverName = "http://192.168.59.1/esp32RemotePhotos/photo_request.php?action=db_check";

const long refreshRate = 5000; // in milliseconds
unsigned long previousMillis = 0; // holds millis value

String requestValue = {};
int LED = 2;

// camera config
static camera_config_t config = {
  .pin_pwdn = PWDN_GPIO_NUM,
  .pin_reset = RESET_GPIO_NUM,
  .pin_xclk = XCLK_GPIO_NUM,
  .pin_sccb_sda = SIOD_GPIO_NUM,
  .pin_sccb_scl = SIOC_GPIO_NUM,
  .pin_d7 = Y9_GPIO_NUM,
  .pin_d6 = Y8_GPIO_NUM,
  .pin_d5 = Y7_GPIO_NUM,
  .pin_d4 = Y6_GPIO_NUM,
  .pin_d3 = Y5_GPIO_NUM,
  .pin_d2 = Y4_GPIO_NUM,
  .pin_d1 = Y3_GPIO_NUM,
  .pin_d0 = Y2_GPIO_NUM,
  .pin_vsync = VSYNC_GPIO_NUM,
  .pin_href = HREF_GPIO_NUM,
  .pin_pclk = PCLK_GPIO_NUM,
  .xclk_freq_hz = 20000000,
  .ledc_timer = LEDC_TIMER_0,
  .ledc_channel = LEDC_CHANNEL_0,
  .pixel_format = PIXFORMAT_JPEG, // format
  .frame_size = FRAMESIZE_UXGA, // resolution
  .jpeg_quality = 13, // quality
  .fb_count = 2,
  .fb_location = CAMERA_FB_IN_PSRAM,
  .grab_mode = CAMERA_GRAB_WHEN_EMPTY
};

// initialize camera with our configuration above
esp_err_t Camera_init(){
  return esp_camera_init(&config);  
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  
  Serial.begin(115200);

  // initialize camera
  esp_err_t camera;
  camera = Camera_init();
  if(camera != ESP_OK){
    Serial.println("Camera init failed");
    return;
  }
  
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

void TakePicture(){
  // we get the frame buffer from camera
  camera_fb_t *fb = NULL;
  fb = esp_camera_fb_get();
  if(!fb){
    Serial.println("ERROR:Frame buffer could not be acquierded!"); 
    return; 
  }
  Serial.println("Success!, frame buffer acquired!");

  char* uploadServer = "http://192.168.59.1/esp32RemotePhotos/photo_upload.php?action=upload&photoString=" + fb->buf;
  
  // return frame buffer back for reuse
  esp_camera_fb_return(fb);
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
        TakePicture();
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
