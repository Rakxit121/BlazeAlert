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
String time;    // Stores time value
float latitude; // Stores latitude value
float longitude; // Stores longitude value

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

  Serial.println("Humidity, Temperatuew,Gas Level,Pressure,Altitude,Date,Time,Latitude,Longitude");

  // Initialize GPS module communication
  ss.begin(115200);
}

void loop() {
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
      // Check if valid date and time data is available
      if (gps.date.isValid() && gps.time.isValid()) {
        // Store date and time values in variables
        date = String(gps.date.year()) + "-" + String(gps.date.month()) + "-" + String(gps.date.day());
        time = String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second());
      }
    }
  }

  // If no GPS data is available, use static values
  if (date.length() == 0 || time.length() == 0 || latitude == 0.0 || longitude == 0.0) {
    date = "2023-06-25";
    time = "12:00:00";
    latitude = 27.70626495563062;
    longitude = 85.3299995345307;
  }

  // Print data to Serial monitor
  Serial.print("#");
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
  Serial.print(time);
  Serial.print(",");
  Serial.print(latitude, 6);
  Serial.print(",");
  Serial.println(longitude, 6);

  delay(1000); // Delay for 1 second before reading again
}
