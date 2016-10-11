/**************************************************************
Zabaleni funkcionality pro WiFiClient (odesilani dat pres HTTPS)
**************************************************************/

#include <WiFiClientSecure.h>
//#include <LedIndicator.h>

class CustomWiFiClient {
public:
   CustomWiFiClient();
   void init();
   void sendData(float temp, float hum);
private:
   char* _host;
   int   _httpPort;
   char* _googleScriptMacroId;
   WiFiClientSecure _client;
 //  LedIndicator     _ledIndicator;
};
