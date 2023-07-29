#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Constants
#define DHT_PIN 2       // DHT22 data pin
#define DHTTYPE DHT22   // DHT 22 (AM2302)
#define MQ3_PIN A0      // MQ3 analog pin
#define SDA_PIN A4      // BMP180 SDA pin
#define SCL_PIN A5      // BMP180 SCL pin
#define GPS_RX_PIN 3    // GPS module receive pin
#define GPS_TX_PIN 4    // GPS module transmit pin

// Initialize sensor objects
DHT dht(DHT_PIN, DHTTYPE);             // DHT22 sensor object
Adafruit_BMP085 bmp;                  // BMP180 sensor object
TinyGPSPlus gps;                      // TinyGPS++ object
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);  // SoftwareSerial object for GPS module

// Variables
float hum;      // Stores humidity value
float temp;     // Stores temperature value
float gasLevel; // Stores gas level value
float pressure; // Stores pressure value
float altitude; // Stores altitude value
String date;    // Stores date value
String timestamp;    // Stores timestamp value
float latitude; // Stores latitude value
float longitude; // Stores longitude value

unsigned long previousMillis = 0;
const unsigned long interval = 20000; // Interval in milliseconds (20 seconds)

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Initialize BMP180 sensor
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
    while (1);
  }

  Serial.println("Humidity, Temperature,Gas Level,Pressure,Altitude,Date,timestamp,Latitude,Longitude");

  // Initialize GPS module communication
  ss.begin(115200);
}

void loop() {
  // Get the current time in milliseconds
  unsigned long currentMillis = millis();

  // Check if the desired interval (20 seconds) has elapsed
  if (currentMillis - previousMillis >= interval) {
    // Save the current time as the last time the data was sent
    previousMillis = currentMillis;

    // Read DHT sensor values
    hum = dht.readHumidity();
    temp = dht.readTemperature();

    // Read gas level from MQ3 sensor
    gasLevel = analogRead(MQ3_PIN);

    // Read BMP180 sensor values
    pressure = bmp.readPressure();
    altitude = bmp.readAltitude();

    // Read GPS data
    while (ss.available() > 0) {
      if (gps.encode(ss.read())) {
        // Check if valid GPS data is available
        if (gps.location.isValid()) {
          // Store longitude and latitude values in variables
          longitude = gps.location.lng();
          latitude = gps.location.lat();
        }
        // Check if valid date and timestamp data is available
        if (gps.date.isValid() && gps.time.isValid()) {
          // Store date and timestamp values in variables
          date = String(gps.date.year()) + "." + String(gps.date.month()) + "." + String(gps.date.day());
          timestamp = String(gps.time.hour()) + "." + String(gps.time.minute()) + "." + String(gps.time.second());
        }
      }
    }

    // If no GPS data is available, use static values
    if (date.length() == 0 || timestamp.length() == 0 || latitude == 0.0 || longitude == 0.0) {
      date = "2023.07.19";
      timestamp = "10.00.00";
      latitude = 27.70626495563062;
      longitude = 85.3299995345307;
    }

    // Print data to Serial monitor
    Serial.print(hum);
    Serial.print(",");
    Serial.print(temp);
    Serial.print(",");
    Serial.print(gasLevel);
    Serial.print(",");
    Serial.print(pressure);
    Serial.print(",");
    Serial.print(altitude);
    Serial.print(",");
    Serial.print(date);
    Serial.print(",");
    Serial.print(timestamp);
    Serial.print(",");
    Serial.print(latitude, 6);
    Serial.print(",");
    Serial.println(longitude, 6);
  }
}
