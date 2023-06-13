#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define DHT_PIN 2      // DHT22 data pin
#define MQ4_PIN A0     // MQ4 analog pin
#define SDA_PIN 20     // BMP180 SDA pin
#define SCL_PIN 21     // BMP180 SCL pin

DHT dht(DHT_PIN, DHT22);              // DHT22 sensor object
Adafruit_BMP085 bmp;                  // BMP180 sensor object

float temperature;
float humidity;
float gasLevel;
float pressure;

void setup() {
  Serial.begin(9600);
  dht.begin();
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  int sensorValue = analogRead(MQ4_PIN);
  gasLevel = map(sensorValue, 0, 1023, 0, 100);
  pressure = bmp.readPressure() / 100.0;

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\t");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");

  Serial.print("Gas Level: ");
  Serial.print(gasLevel);
  Serial.print(" %\t");

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

  delay(2000);
}
