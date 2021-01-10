#include <Adafruit_GFX.h>
//#include <Adafruit_GrayOLED.h>
// #include <Adafruit_SPITFT_Macros.h>
// #include <Adafruit_SPITFT.h>
// #include <gfxfont.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Setup SSD1306 screen
#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET 4        // Reset pin
#define CLK_DURING 800000UL // Speed (in Hz) for Wire transmissions in SSD1306 library calls
TwoWire twi = TwoWire();    // create our own TwoWire instance
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &twi, OLED_RESET, CLK_DURING);

enum ThermostatNames
{
  BASEMENT = 0,
  MAIN_FLOOR,
  UPSTAIRS
};



void setup()
{
  Serial.begin(115200);

  initThermostat(thermostats[BASEMENT], 8, 9, 10);
  initThermostat(thermostats[MAIN_FLOOR], 11, 12, 13);
  initThermostat(thermostats[UPSTAIRS], 14, 15, 16);

  controller.cool1.init(17, HIGH);
  controller.fan.init(18, HIGH);
  controller.heat1.init(19, HIGH);
  controller.heat2.init(20, HIGH);


}

void loop()
{
  delay(2000); //DEBOUNCE

  //read all pin status
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

  HVAC_process(low_heat_state, low_heat_pin_active_level, "Low Heat", low_heat_sent_flag, low_heat_timer, "low_heat", "low_heat_log.php");
  HVAC_process(high_heat_state, high_heat_pin_active_level, "High Heat", high_heat_sent_flag, high_heat_timer, "high_heat", "high_heat_log.php");
  HVAC_process(low_cool_state, low_cool_pin_active_level, "Low Cool", low_cool_sent_flag, low_cool_timer, "low_cool", "low_cool_log.php");
  HVAC_process(high_cool_state, high_cool_pin_active_level, "High Cool", high_cool_sent_flag, high_cool_timer, "high_cool", "high_cool_log.php");
  HVAC_process(fan_state, fan_pin_active_level, "HVAC Fan", fan_sent_flag, fan_timer, "fan", "fan_log.php");
  HVAC_process(humidifier_state, humidifier_pin_active_level, "Humidifier", humidifier_sent_flag, humidifier_timer, "humidifier", "humidifier_log.php");
  HVAC_process(dehumidifier_state, dehumidifier_pin_active_level, "De-humidifier", dehumidifier_sent_flag, dehumidifier_timer, "dehumidifier", "dehumidifier_log.php");
  HVAC_process(filter_state, filter_pin_active_level, "HVAC Filter", filter_sent_flag, filter_timer, "filter", "filter_log.php");
  HVAC_process(estop_state, estop_pin_active_level, "E-Stop", estop_sent_flag, estop_timer, "estop", "estop_log.php");

  Serial.println();
  Serial.println(F("__________________________________________"));
  Serial.println();
}

// Nicely format an IP address.
const char *ip_to_str(const uint8_t *ipAddr)
{
  static char buf[16];
  sprintf(buf, "%d.%d.%d.%d\0", ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]);
  return buf;
}

void HVAC_process(int state, int pin_active_level, const char *message, bool &sent_flag, unsigned long &timer, const char *sql_column, const char *log_php_file)
{
  //for "sql_column" acceptable inputs are "fan", "low_cool", "high_cool", "low_heat", "high_heat", "humidifier", "dehumidifier" without the quotes as those are the column names in the SQL server table
  if (state == pin_active_level)
  { //stage active
    Serial.print(message);
    Serial.println(F(" Pin Active"));
    if (sent_flag == false)
    { //Have not updated server about the pin state yet
      Serial.print(F("Sending "));
      Serial.print(message);
      Serial.println(F(" Pin Active State to server"));
      timer = millis(); //record time when pin went active

      //send message to server that the pin has become active
      if (client.connect(serverip, 80))
      { //
        Serial.print(F("Client Connected - "));
        Serial.print(message);
        Serial.println(F(" state set to 1"));
        client.print(F("GET /~admin/hvac/state.php?column="));
        client.print(sql_column);
        client.print(F("&state=1"));
        client.println(F(" HTTP/1.1"));
        client.println(F("Host: 192.168.1.48"));
        client.println(F("Content-Type: application/x-www-form-urlencoded"));
        client.println(F("Connection: close"));
        client.println();
        client.println();
        client.println(F("Connection: close"));
        client.println();
        client.println();
        client.println(F("Connection: close"));
        client.println();
        client.println();
        client.stop();
        client.stop();
        sent_flag = true; //we sent the message to the server, set the flag so it is not sent again while the pin is active
      }
      else
      {
        Serial.println(F("could not connect to server"));
        sent_flag = false; //because the server was not able to be updated, reset the flag so another attempt can be made to update server
      }
    }
    else
    {
      Serial.print(F("Message already sent to server that "));
      Serial.print(message);
      Serial.println(F(" pin is active"));
    }
  }
  else
  { //the pin is not active
    Serial.print(message);
    Serial.println(F(" Pin is NOT active"));
    if (sent_flag == true)
    { //was it just previously active and a message sent to the server?
      //time to update the server that the pin is no longer active and to send how long the pin was active for

      //update server about pin status
      if (client.connect(serverip, 80))
      {
        Serial.print(F("Client Connected - "));
        Serial.print(message);
        Serial.println(F(" state set to 0"));

        client.print(F("GET /~admin/hvac/state.php?column="));
        client.print(sql_column);
        client.print(F("&state=0"));
        client.println(F(" HTTP/1.1"));
        client.println(F("Host: 192.168.1.48"));
        client.println(F("Content-Type: application/x-www-form-urlencoded"));
        client.println(F("Connection: close"));
        client.println();
        client.println();
        client.println(F("Connection: close"));
        client.println();
        client.println();
        client.println(F("Connection: close"));
        client.println();
        client.println();
        client.stop();
        client.stop();
        sent_flag = false; //reset the message flasg for when the pin becomes active again
      }
      else
      {
        Serial.println(F("could not connect to server"));
        sent_flag = true; //reset the message flag because we still need to tell the server the pin has deactivated
      }

      //send length of time the pin was active
      // duration_clac = millis() - timer;

      if (client.connect(serverip, 80))
      {
        Serial.print(F("Client Connected saving duration "));
        Serial.print(message);
        Serial.println(F(" was on"));
        client.print(F("GET /~admin/hvac/"));
        client.print(log_php_file);
        client.print(F("?duration="));
        // client.print(duration_clac);
        client.println(F(" HTTP/1.1"));
        client.println(F("Host: 192.168.1.48"));
        client.println(F("Content-Type: application/x-www-form-urlencoded"));
        client.println(F("Connection: close"));
        client.println();
        client.println();
        client.println(F("Connection: close"));
        client.println();
        client.println();
        client.println(F("Connection: close"));
        client.println();
        client.println();
        client.stop();
        client.stop();
      }
      else
      {
        Serial.println(F("could not connect to server"));
      }
    }
    else
    {
      //do nothing, pin is not active and the server has been updated
    }
  }
}
