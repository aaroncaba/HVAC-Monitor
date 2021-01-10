#ifndef WIFICONNECT_H_
#define WIFICONNECT_H_

#include <WiFiNINA.h>

class MyWiFi
{
private:
    WiFiClient client;
    char serverName[30];
    String strServerName;

public:
    MyWiFi(){};
    MyWiFi(const String &serverNameIn)
    {
        strServerName = serverNameIn;
        serverNameIn.toCharArray(serverName, 30);
    }
    void printWifiStatus();
    int connectToWifi();
    void sendMessage(String msg);
    bool connected() { return client.connected(); }
    void read()
    {
        while (client.available())
        {
            char c = client.read();
            Serial.write(c);
        }
    }
};

#endif // WIFICONNECT_H_