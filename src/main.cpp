
/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using the WiFi module.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * Board with NINA module (Arduino MKR WiFi 1010, MKR VIDOR 4000 and UNO WiFi Rev.2)

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */

#include <SPI.h>
#include <WiFiNINA.h>

#include "wifiConnect.h"

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "ToshibaLaptop"; // name address for Google (using DNS)

WiFiClient client;

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (connectToWifi() != 0)
  {
    Serial.println("Failed to connect to WiFi");
    while (true) // Pause forever
      ;
  };

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80))
  {
    Serial.println("connected to server");

    // Make a HTTP request:
    // http://<server>/hvac-monitor/addReading.php?signal_id=9&state=off
    client.println("PUT /hvac-monitor/addReading.php?signal_id=9&state=off HTTP/1.1");
    client.println("Host: ToshibaLaptop");
    client.println("Connection: close");
    client.println();
  }
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available())
  {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected())
  {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while (true)
      ;
  }
}
