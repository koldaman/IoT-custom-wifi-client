/**************************************************************
Zabaleni funkcionality pro WiFiClient (odesilani dat pres HTTPS)
**************************************************************/

#include <WiFiClientSecure.h>

class CustomWiFiClient {
public:
   CustomWiFiClient();

   void sendData(float temp, float hum);
   void sentCallback(void (*callback)(int httpStatus));
private:
   char* _host;
   int   _httpPort;
   char* _googleScriptMacroId;
   WiFiClientSecure _client;
   void  (*_callback)(int httpResult);
   int parseHttpResult(String httpResultString);
};
