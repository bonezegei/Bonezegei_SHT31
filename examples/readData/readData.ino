#include <Wire.h>
#include <Bonezegei_SHT31.h>

// Initialize SHT31 with default I2C address (0x44)
Bonezegei_SHT31 sht(0x44);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!sht.begin()) {
    Serial.println("Could not find a valid SHT31 sensor, check wiring!");
    while (1);
  }

  Serial.println("SHT31 Sensor Initialized!");
}

void loop() {
  if (sht.read()) {
    Serial.print("Temperature: ");
    Serial.print(sht.getTemperature());
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(sht.getHumidity());
    Serial.println(" %");

    Serial.println("------------------------------------");
  } else {
    Serial.println("Failed to read from SHT31 sensor!");
  }

  delay(2000);
}