/**************************************************************
  Zabaleni funkcionality pro WiFiClient (odesilani dat pres HTTPS)
 **************************************************************/
#include "Arduino.h"

#include <WiFiClientSecure.h>
#include <CustomWiFiClient.h>
#include <Ticker.h>
//#include <LedIndicator.h>

//const int LED_INDICATOR_PIN = 14;   // D4

CustomWiFiClient::CustomWiFiClient() {
   WiFiClientSecure _client;
//   LedIndicator _ledIndicator;
   _host = "script.google.com";
   _httpPort = 443;
   _googleScriptMacroId = "AKfycbyALzdR-jK6G6WRplkXtwo1izdghNAfYHcPCzYTyrC3j1W5MmZR";
}

void CustomWiFiClient::init() {
//  _ledIndicator.init(LED_INDICATOR_PIN);
}

void CustomWiFiClient::sendData(float temp, float hum) {
   // zablikej behem requestu
//   _ledIndicator.start(0.5);

   if (isnan(temp) || isnan(hum)) {
     Serial.println("Failed to read from DHT sensor!");
 //    _ledIndicator.stop();
     return;
   }

   // volame zabezpecene - WiFiClientSecure, pro obycejne HTTP by stacil WiFiClient
   if (!_client.connect(_host, _httpPort)) {
     Serial.println("connection failed");
 //    _ledIndicator.stop();
     return;
   }

   // knstrukce url
   // priklad: https://script.google.com/macros/s/XxxX/exec?teplota=23.2'&'vlhkost=51.8
   String url = "/macros/s/";
   url += _googleScriptMacroId;
   url += "/exec";
   url += "?teplota=";
   url += temp;
   url += "&vlhkost=";
   url += hum;

   Serial.print("Requesting URL: ");
   Serial.println(url);

   // posli request
   _client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + _host + "\r\n" +
                "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (_client.available() == 0) {
      if (millis() - timeout > 8000) {
        Serial.println(">>> Client Timeout !");
        _client.stop();
//        _ledIndicator.stop();
        return;
      }
    }

    // precteni vystupu volani skriptu
    /*
    while (client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    */

    Serial.println();
    Serial.println("closing connection");

    // konec indikace posilani requestu
 //   _ledIndicator.stop();
}
