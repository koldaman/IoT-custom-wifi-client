/**************************************************************
  Zabaleni funkcionality pro WiFiClient (odesilani dat pres HTTPS)
 **************************************************************/
#include "Arduino.h"

#include <WiFiClientSecure.h>
#include <CustomWiFiClient.h>
#include <Constants.h>

CustomWiFiClient::CustomWiFiClient() {
   WiFiClientSecure _client;
   _host = "script.google.com";
   _httpPort = 443;
   _googleScriptMacroId = Constants::MACRO_ID();
}

void CustomWiFiClient::sendData(float temp, float hum) {
   if (isnan(temp) || isnan(hum)) {
     Serial.println("Failed to read from DHT sensor!");
     if (_callback) {
        _callback(parseHttpResult("FAIL"));
     }
     return;
   }

   // volame zabezpecene - WiFiClientSecure, pro obycejne HTTP by stacil WiFiClient
   if (!_client.connect(_host, _httpPort)) {
     Serial.println("connection failed");
     if (_callback) {
        _callback(parseHttpResult("FAIL"));
     }
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
        return;
      }
    }

    // precteni vystupu volani skriptu
    String firstLine;
    if (_client.available()){
      firstLine = _client.readStringUntil('\r');
    }

    Serial.println("closing connection");

    if (_callback) {
      _callback(parseHttpResult(firstLine));
    }
}

int CustomWiFiClient::parseHttpResult(String httpResultString) {
   // HTTP/1.1 200 - OK
   if (httpResultString.length() > 12) {
      String numericHttpResult = httpResultString.substring(9,12);
      int httpResult = atoi(numericHttpResult.c_str());
      return httpResult == 0 ? -1 : httpResult;
   }
   return -1; // FAILURE
}

void CustomWiFiClient::sentCallback(void (*callback)(int)) {
   _callback = callback;
}
