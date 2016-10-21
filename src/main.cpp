#include "Arduino.h"

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <CustomWiFiClient.h>
#include <Blink.h>

CustomWiFiClient client;

Blink blinker(14 /* PIN */);

float temp;
float hum;

long previousTime = 0;

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
}

void readAndSend() {
   temp = 25;
   hum = 50;

   // logovani posilanych dat
   Serial.print("Temperature: ");
   Serial.print(temp);
   Serial.print("\tHumidity: ");
   Serial.println(hum);

   client.sendData(temp, hum);
}

void handleDataSent(int httpResult) {
   Serial.print("HttpResult: ");
   Serial.print(httpResult);
   if (httpResult >= 200 && httpResult < 300) {
      Serial.println(" - OK");
      blinker.init({10}, 1);
   } else {
      Serial.println(" - FAILED");
      blinker.init({10,50,10,50,10}, 1);
   }
   blinker.start();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);


  //WiFiManage - Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  // wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  IPAddress _ip = IPAddress(10, 0, 0, 1);
  IPAddress _gw = IPAddress(10, 0, 0, 2);
  IPAddress _sn = IPAddress(255, 255, 255, 0);

  wifiManager.setAPStaticIPConfig(_ip, _gw, _sn);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "ESPAutoConnectAP" (with password) and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("ESPAutoConnectAP", "esppassword")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.print("connected to SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("on local ip: ");
  Serial.println(WiFi.localIP());

  client.sentCallback(handleDataSent);
  readAndSend();
}

void loop() {
   unsigned long currentTime = millis();

   int interval = 10*60*1000; // 10 minut

   if (currentTime - previousTime >= interval) {
      readAndSend();
      previousTime = currentTime;
   }
}
