//libraries
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN D2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

#define WIFI_ID "RakshitLaptop"
#define WIFI_PASSWORD "12345678"
#define FIREBASE_HOST "https://blazealert-ac27a-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyD_PRlvDA3Oy3Lh4b6Ku2Joa2ivGdhVZvY"

FirebaseData firebaseData;
WiFiClientSecure wifiClient;

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

// function to delimit the string
String delimit_string(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "-1";
}

void setup()
{
  //Showing in serial monitor
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println();
  WiFi.begin(WIFI_ID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  //  dht.begin();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, wifiClient);
  Firebase.reconnectWiFi(true);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  //  float humidity = dht.readHumidity();
  //  float temperature = dht.readTemperature();
  //
  //  Serial.print("Humidity: ");
  //  Serial.print(humidity);
  //  Serial.print(" %\t");
  //  Serial.print("Temperature: ");
  //  Serial.print(temperature);
  //  Serial.println(" *C");

  if (WiFi.status() == WL_CONNECTED)
  {
    WiFiClient client;
    if (Serial.available())
    {
      if (Serial.read() == '#')
      {
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
        Serial.print(timestamp);
        Serial.print(",");
        Serial.print(latitude, 6);
        Serial.print(",");
        Serial.println(longitude, 6);

        // Store data in Firebase
        String path = "/data";
        String jsonData = "{\"humidity\":" + hum +
                          ",\"temperature\":" + temp +
                          ",\"gasLevel\":" + gasLevel +
                          ",\"pressure\":" + pressure +
                          ",\"altitude\":" + altitude +
                          ",\"date\":\"" + date +
                          "\",\"timestamp\":\"" + timestamp +
                          "\",\"latitude\":" + latitude +
                          ",\"longitude\":" + longitude + "}";
        if (Firebase.pushJSON(firebaseData, path, jsonData))
        {
          Serial.println("Data stored in Firebase");
        }
        else
        {
          Serial.println("Error storing data in Firebase");
          Serial.println(firebaseData.errorReason());
        }
      }
    }
  }

  delay(2000);
}
