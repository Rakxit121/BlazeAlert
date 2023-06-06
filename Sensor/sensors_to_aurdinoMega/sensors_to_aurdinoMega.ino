// Include required libraries
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

 

// Define sensor pin connections
#define DHT_PIN 2      // DHT22 data pin
#define MQ4_PIN A0     // MQ4 analog pin
#define SDA_PIN 20     // BMP180 SDA pin
#define SCL_PIN 21     // BMP180 SCL pin

 

// Initialize sensor objects
DHT dht(DHT_PIN, DHT22);              // DHT22 sensor object
Adafruit_BMP085 bmp;                  // BMP180 sensor object

 

// Variables to store sensor readings
float temperature;
float humidity;
float gasLevel;
float pressure;

 

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

 

  // Initialize DHT22 sensor
  dht.begin();

 

  // Initialize BMP180 sensor
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1);
  }
}

 

void loop() {
  // Read temperature and humidity from DHT22
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

 

  // Read gas level from MQ4 sensor
  int sensorValue = analogRead(MQ4_PIN);
  gasLevel = map(sensorValue, 0, 1023, 0, 100); // Assuming MQ4 provides values from 0 to 1023

 

  // Read pressure from BMP180 sensor
  pressure = bmp.readPressure() / 100.0; // Convert Pa to hPa

 

  // Print sensor readings
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C");
  Serial.print("\t");

 

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %");
  Serial.print("\t");

 

  Serial.print("Gas Level: ");
  Serial.print(gasLevel);
  Serial.print(" %");
  Serial.print("\t");

 

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");

 

  delay(2000); // Delay for 2 seconds between readings
}
