#include <WiFiNINA.h>

#include "ssd1306Display.h"
#include "wifiConnect.h"

// #define SCREEN_WIDTH 128  // OLED display width, in pixels
// #define SCREEN_HEIGHT 64  // OLED display height, in pixels
// #define SSD1306_ADDR 0x3C // SSD1306 display I2C address

// // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// #define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "ToshibaLaptop"; // name address for Google (using DNS)

WiFiClient client;
SsdDisplay display;

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Starting up...");

  if (display.init() != 0)
  {
    Serial.println(F("Initializing display failed"));
    while (true)
      ;
  }

  display.draw_text(0, 0, "Connecting to WiFi", 1);

  if (connectToWifi() != 0)
  {
    Serial.println("Failed to connect to WiFi");
    while (true) // Pause forever
      ;
  };
  display.draw_text(0, 0, "WiFi connected", 1);
  Serial.println("WiFi Connected");

  // Wire.begin();

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
