//libraries
//Nodemcu library
#include <ESP8266WiFi.h>

//Nodemcu Fetch data library
#include <ESP8266HTTPClient.h>

//DHT22 library
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

#define SERVER_IP "192.168.202.82"
#define PORT "5000"
#ifndef WIFI_ID
#define WIFI_ID "RakshitLaptop"
#define WIFI_PASSWORD  "12345678"
#endif



// Variables
String hum = "";      
String temp = "";     
String gasLevel = ""; 
String pressure = "";
String altitude = "";
String date = "";    
String timestamp = "";    
String latitude = ""; 
String longitude = ""; 
String data = "";
int _mode = -1; // 0 check for rfid, 1 check pin




// function to delimit the string
String delimit_string(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "-1";
}

void setup() {
  //Showing in serial monitor
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  WiFi.begin(WIFI_ID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
//  dht.begin();
}
void loop() {
  //  float humidity = dht.readHumidity();
  //  float temperature = dht.readTemperature();
  //
  //  Serial.print("Humidity: ");
  //  Serial.print(humidity);
  //  Serial.print(" %\t");
  //  Serial.print("Temperature: ");
  //  Serial.print(temperature);
  //  Serial.println(" *C");

if ((WiFi.status() == WL_CONNECTED)) {
    WiFiClient client;
    if (Serial.available()) {
      if (Serial.read() == '#') {
        String data = Serial.readStringUntil('\n');
        String hum = delimit_string(data, ',', 0);
        String temp = delimit_string(data, ',', 1);
        String gasLevel = delimit_string(data, ',', 2);
        String pressure = delimit_string(data, ',', 3);
        String altitude = delimit_string(data, ',', 4);
        String date = delimit_string(data, ',', 5);    
        String timestamp = delimit_string(data, ',', 6);    
        String latitude = delimit_string(data, ',', 7);
        String longitude = delimit_string(data, ',', 8); 

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
      }
    }


    //    HTTPClient http;
    //    String toSend = "http://" SERVER_IP ":" PORT "/api/data/";
    //    toSend += String(humidity) + "/"+ String(temperature);
    //    http.begin(client, toSend); //HTTP
    //
    //    int httpResponseCode = http.GET();
    //    if (httpResponseCode > 0) {
    //      const String& payload = http.getString();
    //      Serial.println(payload);
    //    }
    //    else {
    //      Serial.println("#0,0,Request Timeout");
    //    }
    //    http.end();
  }

  delay(5000);
}
