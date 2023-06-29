#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>


// Constants
#define DHT_PIN 2       // DHT22 data pin
#define DHTTYPE DHT22   // DHT 22 (AM2302)
#define MQ3_PIN A0      // MQ3 analog pin
#define SDA_PIN 20      // BMP180 SDA pin
#define SCL_PIN 21      // BMP180 SCL pin

// Initialize sensor objects
DHT dht(DHT_PIN, DHTTYPE);             // DHT22 sensor object
Adafruit_BMP085 bmp;                  // BMP180 sensor object


// Variables
float hum;      // Stores humidity value
float temp;     // Stores temperature value
float gasLevel; // Stores gas level value
float pressure; // Stores pressure value
float altitude; // Stores altitude value

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

  Serial.println("Humidity, Temperatuew,Gas Level,Pressure,Altitude");


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
  Serial.println(altitude);

  delay(1000); // Delay for 1 second before reading again
}
