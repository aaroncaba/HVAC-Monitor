#ifndef DEVICES_H_
#define DEVICES_H_

#include <Arduino.h>

enum ThermostatNames
{
  BASEMENT = 0,
  MAIN_FLOOR,
  UPSTAIRS
};

class Signal
{

public:
  String name;
  // ON or OFF
  PinStatus state;
  PinStatus active_level;

  //if the pin changes state, send a message to the server once and set the flag to prevent message from sending again
  bool sent_flag = false;

  unsigned pin = 0;

  void readPin()
  {
    PinStatus currentState = digitalRead(pin);
    if (currentState != state)
    {
      state = currentState;
      sent_flag = false;
    }
  }

  void init(const String &name_in, const int &pin, const PinStatus &active_level_in)
  {
    this->pin = pin;
    pinMode(pin, INPUT);
    this->active_level = active_level_in;
    if (this->active_level == HIGH)
    {
      this->state = LOW;
    }
    else
    {
      this->state = HIGH;
    }
    this->name = name_in;
  }
};

class Thermostat
{
public:
  Thermostat(){};
  Thermostat(const String &name, const int &cool_pin, const int &heat_pin, const int &fan_pin)
  {
    this->name = name;
    this->cool.init("Cool1", cool_pin, HIGH);
    this->heat.init("Heat1", heat_pin, HIGH);
    this->fan.init("Fan", fan_pin, HIGH);
  }
  String name;
  Signal cool;
  Signal heat;
  Signal fan;
};

class Controller
{
public:
  Controller(){};
  Controller(const String &name, const int &cool_pin, const int &heat1_pin, const int &heat2_pin, const int &fan_pin)
  {
    this->name = name;
    this->cool1.init("Cool1", cool_pin, HIGH);
    this->heat1.init("Heat1", heat1_pin, HIGH);
    this->heat2.init("Heat2", heat2_pin, HIGH);
    this->fan.init("Fan", fan_pin, HIGH);
  };
  String name;
  Signal cool1;
  Signal heat1;
  Signal heat2;
  Signal fan;
};

#endif