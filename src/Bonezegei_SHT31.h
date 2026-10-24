/*
  Library for SHT31 Sensor
  Author: Jofel Batutay (Bonezegei)
  Date: October 24, 2026
*/

#ifndef BONEZEGEI_SHT31_H
#define BONEZEGEI_SHT31_H

#include <Arduino.h>
#include <Wire.h>

#define SHT31_I2C_ADDR_PRIMARY   0x44
#define SHT31_I2C_ADDR_SECONDARY 0x45

#define SHT31_CMD_MEAS_HIGH_REP  0x2400
#define SHT31_CMD_SOFT_RESET     0x30A2
#define SHT31_CMD_READ_STATUS    0xF32D

class Bonezegei_SHT31 {
public:
  Bonezegei_SHT31(uint8_t addr = SHT31_I2C_ADDR_PRIMARY);
  
  bool begin(TwoWire &wirePort = Wire);
  bool read();
  
  float getTemperature();  // Returns Temperature in Celsius
  float getHumidity();     // Returns Relative Humidity in %

private:
  uint8_t _addr;
  TwoWire *_wire;

  float _temperature;
  float _humidity;

  void writeCommand(uint16_t cmd);
  uint8_t crc8(const uint8_t *data, uint8_t len);
};

#endif