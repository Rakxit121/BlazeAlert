#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

const char* WIFI_ID = "RakshitLaptop";
const char* WIFI_PASSWORD = "12345678";

unsigned long myChannelNumber = 2206646;
const char* myWriteAPIKey = "0LR4ATPF53MQ5BYB";

float humidity = 0.0;
float temperature = 0.0;
int gasLevel = 0;
float pressure = 0.0;
float altitude = 0.0;
String date = "";
String timestamp = "";
float longitude = 0.0;
float latitude = 0.0;

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(WIFI_ID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  ThingSpeak.begin(client);
}

// Function to delimit the string
String delimit_string(String data, char separator, int index) {
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

// Parse and store the data
void parseData(String receivedData) {
  humidity = delimit_string(receivedData, ',', 0).toFloat();
  temperature = delimit_string(receivedData, ',', 1).toFloat();
  gasLevel = delimit_string(receivedData, ',', 2).toInt();
  pressure = delimit_string(receivedData, ',', 3).toFloat();
  altitude = delimit_string(receivedData, ',', 4).toFloat();
  date = delimit_string(receivedData, ',', 5);
  timestamp = delimit_string(receivedData, ',', 6);
  longitude = delimit_string(receivedData, ',', 7).toFloat();
  latitude = delimit_string(receivedData, ',', 8).toFloat();
}

void loop() {
  // Assuming you receive the data and store it in a string variable called 'receivedData'
  String receivedData = ""; // Initialize an empty string

  // Read serial input until a newline character is encountered
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      break; // Exit the loop if newline character is encountered
    }
    receivedData += c; // Append the character to the receivedData string
  }

  // Check if receivedData is not empty
  if (receivedData.length() > 0) {
    receivedData.trim(); // Remove leading/trailing whitespaces
    Serial.println("Received data: " + receivedData);

    // Call the parseData function to extract values and store them in variables
    parseData(receivedData);

    // Update the fields with the parsed data
    ThingSpeak.setField(1, String(humidity));
    ThingSpeak.setField(2, String(temperature));
    ThingSpeak.setField(3, String(gasLevel));
    ThingSpeak.setField(4, String(pressure));
    ThingSpeak.setField(5, String(altitude));
    ThingSpeak.setField(6, date);
    ThingSpeak.setField(7, timestamp);
    ThingSpeak.setField(8, String(longitude));
    ThingSpeak.setField(9, String(latitude));

    // Write the data to ThingSpeak
    int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if (response == 200) {
      Serial.println("Data sent to ThingSpeak successfully");
    } else {
      Serial.print("Error sending data to ThingSpeak. Response code: ");
      Serial.println(response);
    }
  }

  // Wait for 10 seconds before reading the next set of data
  delay(20000);
}







//#include <ESP8266WiFi.h>
//#include <ThingSpeak.h>
//
//const char* WIFI_ID = "RakshitLaptop";
//const char* WIFI_PASSWORD = "12345678";
//
//unsigned long myChannelNumber = 2206646;
//const char* myWriteAPIKey = "0LR4ATPF53MQ5BYB";
//
//float humidity = 0.0;
//float temperature = 0.0;
//int gasLevel = 0;
//float pressure = 0.0;
//float altitude = 0.0;
//String date = "";
//String timestampstamp = "";
//float longitude = 0.0;
//float latitude = 0.0;
//
//WiFiClient client;
//
//void setup() {
//  Serial.begin(115200);
//
//  // Connect to Wi-Fi
//  WiFi.begin(WIFI_ID, WIFI_PASSWORD);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(1000);
//    Serial.println("Connecting to WiFi...");
//  }
//  Serial.println("Connected to WiFi");
//
//  ThingSpeak.begin(client);
//}
//
//// Function to delimit the string
//String delimit_string(String data, char separator, int index) {
//  int found = 0;
//  int strIndex[] = { 0, -1 };
//  int maxIndex = data.length() - 1;
//
//  for (int i = 0; i <= maxIndex && found <= index; i++) {
//    if (data.charAt(i) == separator || i == maxIndex) {
//      found++;
//      strIndex[0] = strIndex[1] + 1;
//      strIndex[1] = (i == maxIndex) ? i + 1 : i;
//    }
//  }
//  return found > index ? data.substring(strIndex[0], strIndex[1]) : "-1";
//}
//
//// Parse and store the data
//void parseData(String receivedData) {
//  humidity = delimit_string(receivedData, ',', 0).toFloat();
//  temperature = delimit_string(receivedData, ',', 1).toFloat();
//  gasLevel = delimit_string(receivedData, ',', 2).toInt();
//  pressure = delimit_string(receivedData, ',', 3).toFloat();
//  altitude = delimit_string(receivedData, ',', 4).toFloat();
////  date = delimit_string(receivedData, ',', 5);
////  timestampstamp = delimit_string(receivedData, ',', 6);
////  longitude = delimit_string(receivedData, ',', 7).toFloat();
////  latitude = delimit_string(receivedData, ',', 8).toFloat();
//}
//
//void loop() {
//  // Assuming you receive the data and store it in a string variable called 'receivedData'
//  String receivedData = ""; // Initialize an empty string
//
//  // Check if data is available to read from the serial port
//  if (Serial.available()) {
//    receivedData = Serial.readStringUntil('\n'); // Read the data until newline character
//    receivedData.trim(); // Remove leading/trailing whitespaces
//    Serial.println("Received data: " + receivedData);
//
//    // Call the parseData function to extract values and store them in variables
//    parseData(receivedData);
//
//    // Update the fields with the parsed data
//    ThingSpeak.setField(1, String(humidity));
//    ThingSpeak.setField(2, String(temperature));
//    ThingSpeak.setField(3, String(gasLevel));
//    ThingSpeak.setField(4, String(pressure));
//    ThingSpeak.setField(5, String(altitude));
////    ThingSpeak.setField(6, date);
////    ThingSpeak.setField(7, timestampstamp);
////    ThingSpeak.setField(8, String(longitude));
////    ThingSpeak.setField(9, String(latitude));
//
//    // Write the data to ThingSpeak
//    int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
//
//    if (response == 200) {
//      Serial.println("Data sent to ThingSpeak successfully");
//    } else {
//      Serial.print("Error sending data to ThingSpeak. Response code: ");
//      Serial.println(response);
//    }
//  }
//
//  // Wait for 10 seconds before sending the next set of data
//  delay(15000);
//}















//#include <ESP8266WiFi.h>
//#include <ThingSpeak.h>
//#include <DHT.h>
//#include <Adafruit_Sensor.h>
//#include <ESP8266HTTPClient.h>
//
//#define DHTPIN D2
//#define DHTTYPE DHT22
//
//DHT dht(DHTPIN, DHTTYPE);
//
//const char* ssid = "RakshitLaptop";   // your network SSID (name)
//const char* password = "12345678";   // your network password
//
//WiFiClient client;
//
//unsigned long myChannelNumber = 2206646;
//const char* myWriteAPIKey = "0LR4ATPF53MQ5BYB";
//
//unsigned long lasttimestampstamp = 0;
//unsigned long timestampstamprDelay = 30000;
//
//void setup() {
//  Serial.begin(115200);
//  Serial.println();
//  Serial.println("Connecting to WiFi...");
//
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());
//
//  ThingSpeak.begin(client);
//  dht.begin();
//}
//
//void loop() {
//  if ((millis() - lasttimestampstamp) > timestampstamprDelay) {
//    if (WiFi.status() != WL_CONNECTED) {
//      Serial.print("Attempting to connect");
//      while (WiFi.status() != WL_CONNECTED) {
//        WiFi.begin(ssid, password);
//        delay(5000);
//      }
//      Serial.println("\nConnected.");
//    }
//
//    float humidity = dht.readHumidity();
//    float temperature = dht.readTemperature();
//    float gasLevel = getGasLevel();
//    float pressure = getPressure();
//    float altitude = getAltitude();
//    String date = getCurrentDate();
//    String timestampstamp = getCurrenttimestampstamp();
//    String latitude = getLatitude();
//    String longitude = getLongitude();
//
////    Serial.print("Humidity: ");
////    Serial.println(humidity);
////    Serial.print("Temperature: ");
////    Serial.println(temperature);
////    Serial.print("Gas Level: ");
////    Serial.println(gasLevel);
////    Serial.print("Pressure: ");
////    Serial.println(pressure);
////    Serial.print("Altitude: ");
////    Serial.println(altitude);
////    Serial.print("Date: ");
////    Serial.println(date);
////    Serial.print("timestampstamp: ");
////    Serial.println(timestampstamp);
////    Serial.print("Latitude: ");
////    Serial.println(latitude);
////    Serial.print("Longitude: ");
////    Serial.println(longitude);
//
//    ThingSpeak.setField(1, humidity);       // Humidity
//    ThingSpeak.setField(2, temperature);    // Temperature
//    ThingSpeak.setField(3, gasLevel);       // Gas Level
//    ThingSpeak.setField(4, pressure);       // Pressure
//    ThingSpeak.setField(5, altitude);       // Altitude
//    ThingSpeak.setField(6, date);           // Date
//    ThingSpeak.setField(7, timestampstamp);           // timestampstamp
//    // ThingSpeak.setField(8, latitude);       // Latitude
//    // ThingSpeak.setField(9, longitude);      // Longitude
//
//    int statusCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
//
//    if (statusCode == 200) {
//      Serial.println("Channel update successful.");
//    }
//    else {
//      Serial.print("Problem updating channel. HTTP error code: ");
//      Serial.println(statusCode);
//    }
//
//    lasttimestampstamp = millis();
//  }
//}
//float getHumidity(float humidity) {
//  // Retrieve the gas level value
//  // Replace with your code
//  return humidity;
//}
//
//float getTemperature(float temperature) {
//  // Retrieve the gas level value
//  // Replace with your code
//  return temperature;
//}
//// Placeholder functions, replace with your actual implementations
//float getGasLevel() {
//  // Retrieve the gas level value
//  // Replace with your code
//  return gasLevel; // Example value
//}
//
//float getPressure() {
//  // Retrieve the pressure value
//  // Replace with your code
//  // Implement your pressure retrieval logic here
//  return pressure; // Example value
//}
//
//float getAltitude() {
//  // Retrieve the altitude value
//  // Replace with your code
//  // Implement your altitude retrieval logic here
//  return altitude; // Example value
//}
//
//String getCurrentDate() {
//  // Retrieve the current date
//  // Replace with your code
//  return ""; // Example value
//}
//
//String getCurrenttimestampstamp() {
//  // Retrieve the current timestampstamp
//  // Replace with your code
//  return ""; // Example value
//}
//
//String getLatitude() {
//  // Retrieve the latitude value
//  // Replace with your code
//  return ""; // Example value
//}
//
//String getLongitude() {
//  // Retrieve the longitude value
//  // Replace with your code
//  return ""; // Example value
//}
