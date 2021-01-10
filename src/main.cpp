#include <WiFiNINA.h>

#include "ssd1306Display.h"
#include "wifiConnect.h"
#include "devices.h"

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

MyWiFi client;
SsdDisplay display;
Thermostat thermostats[3];
const byte thermostatCount = sizeof(thermostats) / sizeof(*thermostats);
Controller controller;

void HVAC_process(const String &deviceName, Signal signal)
{
  if (!signal.sent_flag)
  {
  }
}

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Starting up...");

  // Display intialization
  if (display.init() != 0)
  {
    Serial.println(F("Initializing display failed"));
    while (true)
      ;
  }

  // Devices initilization
  display.display.clearDisplay();
  display.draw_text(0, 0, "Init Devices", 1);
  thermostats[BASEMENT].init(2, 3, 4);
  thermostats[MAIN_FLOOR].init(5, 6, 7);
  thermostats[UPSTAIRS].init(8, 9, 10);
  controller.init(11, 12, 14, 15);

  // WiFi intilization
  display.display.clearDisplay();
  display.draw_text(0, 0, "Connecting to WiFi", 1);
  client = MyWiFi("ToshibaLaptop");
  if (client.connectToWifi() != 0)
  {
    Serial.println("Failed to connect to WiFi");
    while (true) // Pause forever
      ;
  };
  display.display.clearDisplay();
  display.draw_text(0, 0, "WiFi connected", 1);
  Serial.println("WiFi Connected");

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  client.sendMessage("PUT /hvac-monitor/addReading.php?signal_id=9&state=off");
}

void loop()
{
  delay(1000); //DEBOUNCE

  for (int i = 0; i < thermostatCount; i++)
  {
    thermostats[i].cool.readPin();
    thermostats[i].heat.readPin();
    thermostats[i].fan.readPin();
  }
  controller.cool1.readPin();
  controller.fan.readPin();
  controller.heat1.readPin();
  controller.heat2.readPin();

  if (thermostats[0].cool.state == thermostats[0].cool.active_level)
  {
    display.display.clearDisplay();
    display.draw_text(0, 0, "Active", 1);
  }
  else
  {
    display.display.clearDisplay();
    display.draw_text(0, 0, "Not Active", 1);
  }

  // if there are incoming bytes available
  // from the server, read them and print them:
  client.read();

  // if the server's disconnected, stop the client:
  if (!client.connected())
  {
    Serial.println();
    Serial.println("disconnecting from server.");
    // client.stop();

    // do nothing forevermore:
    while (true)
      ;
  }
}
