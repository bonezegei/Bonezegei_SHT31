/*
  Library for SHT31 Sensor
  Author: Jofel Batutay (Bonezegei)
  Date: October 24, 2026
*/

#include "Bonezegei_SHT31.h"

Bonezegei_SHT31::Bonezegei_SHT31(uint8_t addr) {
  _addr = addr;
  _temperature = 0.0;
  _humidity = 0.0;
}

bool Bonezegei_SHT31::begin(TwoWire &wirePort) {
  _wire = &wirePort;
  _wire->begin();

  // Soft Reset
  writeCommand(SHT31_CMD_SOFT_RESET);
  delay(10);

  // Check communication by requesting status register
  writeCommand(SHT31_CMD_READ_STATUS);
  _wire->requestFrom(_addr, (uint8_t)3);
  if (_wire->available() < 3) {
    return false;
  }

  uint8_t status[3];
  status[0] = _wire->read();
  status[1] = _wire->read();
  status[2] = _wire->read();

  // Validate status register CRC
  if (crc8(status, 2) != status[2]) {
    return false;
  }

  return true;
}

bool Bonezegei_SHT31::read() {
  // High-repeatability single measurement command without clock stretching
  writeCommand(SHT31_CMD_MEAS_HIGH_REP);
  delay(15);

  _wire->requestFrom(_addr, (uint8_t)6);
  if (_wire->available() < 6) return false;

  uint8_t buf[6];
  for (uint8_t i = 0; i < 6; i++) {
    buf[i] = _wire->read();
  }

  // Validate CRCs for Temperature and Humidity data
  if (crc8(&buf[0], 2) != buf[2]) return false;
  if (crc8(&buf[3], 2) != buf[5]) return false;

  uint16_t rawTemp = (buf[0] << 8) | buf[1];
  uint16_t rawHum  = (buf[3] << 8) | buf[4];

  _temperature = -45.0f + (175.0f * ((float)rawTemp / 65535.0f));
  _humidity    = 100.0f * ((float)rawHum / 65535.0f);

  return true;
}

float Bonezegei_SHT31::getTemperature() { return _temperature; }
float Bonezegei_SHT31::getHumidity() { return _humidity; }

void Bonezegei_SHT31::writeCommand(uint16_t cmd) {
  _wire->beginTransmission(_addr);
  _wire->write((uint8_t)(cmd >> 8));
  _wire->write((uint8_t)(cmd & 0xFF));
  _wire->endTransmission();
}

uint8_t Bonezegei_SHT31::crc8(const uint8_t *data, uint8_t len) {
  uint8_t crc = 0xFF;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint8_t bit = 8; bit > 0; --bit) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;
      } else {
        crc = (crc << 1);
      }
    }
  }
  return crc;
}