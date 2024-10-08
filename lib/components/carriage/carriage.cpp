#include "carriage.h"

Carriage::Carriage() { reset(); }

void Carriage::reset() {
  _type = CarriageType::NoCarriage;
  _position = 0;  // doesn't matter a priori
}

bool Carriage::isCrossing(HallSensor *sensor, Direction requestedDirection) {
  // offset = # of needles elapsed since detection
  uint8_t offset = (uint8_t)(_position - sensor->getDetectedPosition());
  // direction derived from offset (manipulated as int8)
  Direction direction = (offset < 128) ? Direction::Right : Direction::Left;

  if (direction == requestedDirection) {
    _type = sensor->getDetectedCarriage();
    _position = sensor->getSensorPosition() + offset;
    if (_type == CarriageType::Gartner) {
      // Inner magnets are +/-12 needles fsrom the center
      _position = direction == Direction::Left ? (uint8_t)(_position  + 12) : (uint8_t)(_position - 12);
    }
    return true;
  }
  return false;
}

bool Carriage::isDefined() { return _type != CarriageType::NoCarriage; }

CarriageType Carriage::getType() { return _type; }

void Carriage::setPosition(uint8_t position) { _position = position; }

uint8_t Carriage::getPosition() { return _position; }

uint8_t Carriage::getSelectPosition(Direction direction) {
  // Selection you take place 12 before, 4 after the needle checker (NC)
  // position
  switch (_type) {
    case CarriageType::Gartner:
      // G: NC @ +0/Left, -0/Right and magnets @-12 and +12
      // selection @ -12/Left, +12/Right NOK as it may overlap with
      // carriage detection for wide patterns using needles -100/-99/99/100
      // 8 looks marginal => selection @ -6/Left, +6/Right (head/6 tail/10)
      return (direction == Direction::Left) ? (uint8_t)(_position - 6)
                                            : (uint8_t)(_position + 6);
      break;
    case CarriageType::Lace:
      // L: NC @ +12/Left, -12/Right and magnet @ 0
      // selection @ -0/Left, +0/Right NOK as it may overlap with
      // carriage detection for wide patterns using needles -100/-99/99/100
      // => selection @ +4/Left, -4/Right (head/8 tail/8)
      return (direction == Direction::Left) ? (uint8_t)(_position + 4)
                                            : (uint8_t)(_position - 4);
      break;
    default:  // CarriageType::Knit
      // K: NC @ +24/Left, -24/Right and magnet @ 0
      // carriage detection always far away from NC
      // => selection @ +12/Left, -12/Right (head/12 tail/4)
      return (direction == Direction::Left) ? (uint8_t)(_position + 12)
                                            : (uint8_t)(_position - 12);
      break;
  }
}