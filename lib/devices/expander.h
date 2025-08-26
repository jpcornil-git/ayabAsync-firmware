#ifndef EXPANDER_H
#define EXPANDER_H

#include "hal.h"

#define MCP23008_OLAT 0x0A
#define MCP23008_IODIR 0x00

typedef enum{
    MCP23008,
    PCF8574, 
    PCF8574A,
}io_expander_t;

class Expander {
    public:
    Expander(hardwareAbstraction::HalInterface *hal, const uint8_t i2cAddress, io_expander_t expanderType);
    ~Expander() = default;

    void write(uint8_t address, uint8_t value);

    io_expander_t expanderType;

    private:
    hardwareAbstraction::HalInterface *_hal;
    uint8_t _i2cAddress;
    uint8_t _last_output;
    bool _last_output_valid;
};

#endif