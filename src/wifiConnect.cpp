#include "wifiConnect.h"

#include <Arduino.h>

#include "arduino_secrets.h"

void MyWiFi::printWifiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

int MyWiFi::connectToWifi()
{
    ///////please enter your sensitive data in the Secret tab/arduino_secrets.h
    char ssid[] = SECRET_SSID; // your network SSID (name)
    char pass[] = SECRET_PASS; // your network password (use for WPA, or use as key for WEP)

    int status = WL_IDLE_STATUS;
    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("Communication with WiFi module failed!");
        return -1;
    }

    String fv = WiFi.firmwareVersion();
    if (fv < WIFI_FIRMWARE_LATEST_VERSION)
    {
        Serial.println("Please upgrade the firmware");
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait for connection:
        delay(1000);
    }
    Serial.println("Connected to wifi");
    printWifiStatus();
    return 0;
}

void MyWiFi::sendMessage(String msg)
{

    if (client.connect(serverName, 80))
    {
        Serial.println("connected to server");

        // Make a HTTP request:
        client.println(msg + " HTTP/1.1");
        client.println("Host: " + strServerName);
        client.println("Connection: close");
        client.println();
    }

    else
    {
        Serial.println(F("could not connect to server"));
    }
}