#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Define sensor pin connections
#define DHT_PIN 2      // DHT22 data pin
#define MQ4_PIN A0     // MQ4 analog pin
#define SDA_PIN A4     // BMP180 SDA pin
#define SCL_PIN A5     // BMP180 SCL pin
#define GPS_RX_PIN 3   // GPS software serial RX pin
#define GPS_TX_PIN 4   // GPS software serial TX pin

// Initialize sensor objects
DHT dht(DHT_PIN, DHT22);              // DHT22 sensor object
Adafruit_BMP085 bmp;                  // BMP180 sensor object

// SoftwareSerial object for NodeMCU communication
SoftwareSerial nodeMCU(0, 18); // RX0, TX1 pins

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port for GPS communication
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);

// Variables to store sensor readings
float temperature;
float humidity;
float gasLevel;
float pressure;
float altitude;

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

  // Initialize NodeMCU communication
  nodeMCU.begin(9600);

  // Initialize GPS communication
  gpsSerial.begin(9600);
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

  // Read altitude from BMP180 sensor
  altitude = bmp.readAltitude();

  // Print sensor data in CSV format
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.print(gasLevel);
  Serial.print(",");
  Serial.print(pressure);
  Serial.print(",");
  Serial.println(altitude);

  // Prepare the sensor data in CSV format
  String sensorData = String(temperature) + "," + String(humidity) + "," + String(gasLevel) + "," +
                      String(pressure) + "," + String(altitude);

  // Send sensor data to NodeMCU via SoftwareSerial
  nodeMCU.println(sensorData);

  // Read GPS data
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      displayInfo();
    }
  }

  // Delay for 2 seconds between readings
  delay(2000);
}

void displayInfo() {
  if (gps.location.isValid()) {
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude: ");
    Serial.println(gps.altitude.meters());
  } else {
    Serial.println("Location: Not Available");
  }

  Serial.print("Date: ");
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  } else {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print("0");
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print("0");
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print("0");
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print("0");
    Serial.println(gps.time.centisecond());
  } else {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
}
