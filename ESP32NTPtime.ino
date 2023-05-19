// ESP32NTPtime sync NTP server time to ESP32 local time
// and print ESP32 local time every 1 minute.
//
// By Rodney Tan (PhD)
// Version 1.00 (May 2023)

#include <WiFi.h>
#include "time.h" // ESP32 local time library

const char* ssid     = "ssid";
const char* password = "password";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 28800;  //GMT+8:00
const int   daylightOffset_sec = 0;

void ConnectWiFi(){
  // Connecting to WiFi
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
}

void SyncNTP(){
  // Getting NTP server time
  Serial.print("Getting NTP server time.");

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Successfully synced NTP server time to ESP32 local time");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup(){
  Serial.begin(115200);
  // Connect to WiFi
  ConnectWiFi();
  // Get NTP server time
  SyncNTP();
}

void loop(){
  // Print ESP32 internal RTC every 1 minute
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  if (timeinfo.tm_sec==0){
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    delay(1500); // prevent double printing
  }
}
