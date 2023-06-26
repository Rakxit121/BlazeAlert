
//Libraries
#include <DHT.h>;
const int MQ3pin = A0;
float sensorValue;
//Constants
#define DHTPIN 2         // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
const int sensor_pin = A1;
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

void setup()
{
  Serial.begin(115200);
  dht.begin();
}

void loop()
{
  delay(2000);
  float moisture_percentage;
  int sensor_analog;
  sensor_analog = analogRead(sensor_pin);
  moisture_percentage = ( 100 - ( (sensor_analog / 1023.00) * 100 ) );

  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.print("#");
  Serial.print(hum);Serial.print(",");
  Serial.print(temp);Serial.print(",");
  Serial.print(sensor_analog);Serial.print(",");
  Serial.print(moisture_percentage);Serial.println();
  //Print temp and humidity values to serial monitor
//  Serial.print("Humidity: ");
//  Serial.print(hum);
//  Serial.print(" %, Temp: ");
//  Serial.print(temp);
//  Serial.print(" Celsius");
//
//
//  Serial.print("Moisture Percentage = ");
//  Serial.println(moisture_percentage);
//
//  sensorValue = analogRead(MQ3pin); // read analog input pin 0
//
//  Serial.print("Sensor Value: ");
//  Serial.println(sensorValue);

  delay(2000); // wait 2s for next reading

}
