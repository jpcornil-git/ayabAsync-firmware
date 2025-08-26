#include "solenoids.h"

Solenoids::Solenoids(hardwareAbstraction::HalInterface *hal,
                     const uint8_t i2cAddress[2], io_expander_t expanderType) {
  for (int i = 0; i < 2; i++) {
    _expander[i] = new Expander(hal, i2cAddress[i], expanderType);

    if(expanderType == MCP23008){
      _expander[i]->write(MCP23008_IODIR, 0);  // Configure as output
    }
  }
  reset();
}

Solenoids::Solenoids(hardwareAbstraction::HalInterface *hal, io_expander_t expanderType) {
  for (int i = 0; i < 2; i++) {
    switch (expanderType)
    {
    case MCP23008:
      _expander[i] = new Expander(hal, mcp23008_addrs[i], expanderType);
      break;
    
    case PCF8574:
        _expander[i] = new Expander(hal, pcf8574_addrs[i], expanderType);
      break;

    case PCF8574A:
      _expander[i] = new Expander(hal, pcf8574a_addrs[i], expanderType);
      break;

    default:
      break;
    }

    if(expanderType == MCP23008)
      _expander[i]->write(MCP23008_IODIR, 0);  // Configure as output
  }
}

void Solenoids::reset() {
  set(SOLENOID_RESET_STATE);
}

void Solenoids::reset(uint8_t solenoid) {
  set(solenoid, (bool)SOLENOID_RESET_STATE);
}

void Solenoids::set(uint16_t states) {
  _states = states;
  _updateDevices();
}

void Solenoids::set(uint8_t solenoid, bool state) {
  if (state) {
    _states |= (1 << solenoid);
  } else {
    _states &= ~(1 << solenoid);
  }
  _updateDevices();
}

void Solenoids::_updateDevices() {
  uint16_t values = _states;
  for (int i = 0; i < 2; i++) {
    _expander[i]->write(MCP23008_OLAT, (uint8_t)(values & 0xff));
    values >>= 8;
  }
}