#include "expander.h"

Expander::Expander(hardwareAbstraction::HalInterface *hal, const uint8_t i2cAddress, io_expander_t expanderType) {
  _hal = hal;
  _i2cAddress = i2cAddress;
  _last_output_valid = false;
  expanderType = expanderType;
}

void Expander::write(uint8_t address, uint8_t value) {
  if (address == MCP23008_OLAT) {
    if ((value == _last_output) && (_last_output_valid)) {
      return;
    }
    _last_output = value;
    _last_output_valid = true;
  }

switch (expanderType)
{
case MCP23008:
     _hal->i2c->write(_i2cAddress, address, value);
    break;

case PCF8574:
case PCF8574A:
    _hal->i2c->write(_i2cAddress, value);
    
default:
    break;
}
 
}