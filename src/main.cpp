#include <WiFiNINA.h>

#include "ssd1306Display.h"
#include "wifiConnect.h"
#include "devices.h"

MyWiFi client;
SsdDisplay display;
Thermostat thermostats[3];
const byte thermostatCount = sizeof(thermostats) / sizeof(*thermostats);
Controller controller;

void HVAC_process(const String &deviceName, Signal &signal)
{
  if (!signal.sent_flag)
  {
    String state = (signal.state == signal.active_level) ? "ON" : "OFF";
    Serial.println("Sending " + deviceName + " " + signal.name + " " + state);
    String msg = "PUT /hvac-monitor/addReading.php?signal_id=9&state=" + state;
    if (client.sendMessage(msg) == 0)
    {
      Serial.println("Message sent");
      signal.sent_flag = true;
    }
    else
    {
      Serial.println(F("Message not sent"));
    }
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
  // Names must match the device names in the SQL database
  thermostats[BASEMENT] = Thermostat("Basement", 2, 3, 4);
  thermostats[MAIN_FLOOR] = Thermostat("Main_Floor", 5, 6, 7);
  thermostats[UPSTAIRS] = Thermostat("Upstairs", 8, 9, 10);
  controller = Controller("HVAC_Controller", 11, 12, 14, 15);

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
}

void loop()
{
  delay(1000); //DEBOUNCE

  Serial.println("Reading Pins");
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

  for (int i = 0; i < thermostatCount; i++)
  {
    HVAC_process(thermostats[i].name, thermostats[i].cool);
    HVAC_process(thermostats[i].name, thermostats[i].heat);
    HVAC_process(thermostats[i].name, thermostats[i].fan);
  }
    HVAC_process(controller.name, controller.heat1);
    HVAC_process(controller.name, controller.heat2);
    HVAC_process(controller.name, controller.cool1);
    HVAC_process(controller.name, controller.fan);

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
  // if (!client.connected())
  // {
  //   Serial.println();
  //   Serial.println("disconnecting from server.");
  //   // client.stop();

  //   // do nothing forevermore:
  //   while (true)
  //     ;
  // }
}
