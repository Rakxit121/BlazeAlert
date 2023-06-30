#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

const char* WIFI_ID = "RakshitLaptop";
const char* WIFI_PASSWORD = "12345678";

unsigned long myChannelNumber = 2207955;
const char* myWriteAPIKey = "7X6XYMLH9HPA8U9Q";

float humidity = 0.0;
float temperature = 0.0;
int gasLevel = 0;
float pressure = 0.0;
float altitude = 0.0;

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

// Parse and store the data
void parseData(String receivedData) {
  humidity = delimit_string(receivedData, ',', 0).toFloat();
  temperature = delimit_string(receivedData, ',', 1).toFloat();
  gasLevel = delimit_string(receivedData, ',', 2).toInt();
  pressure = delimit_string(receivedData, ',', 3).toFloat();
  altitude = delimit_string(receivedData, ',', 4).toFloat();
}

void loop() {
  // Assuming you receive the data and store it in a string variable called 'receivedData'
  String receivedData = ""; // Initialize an empty string

  // Check if data is available to read from the serial port
  if (Serial.available()) {
    receivedData = Serial.readStringUntil('\n'); // Read the data until newline character
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

    // Write the data to ThingSpeak
    int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if (response == 200) {
      Serial.println("Data sent to ThingSpeak successfully");
//      Serial.println("Received data: " + receivedData);
    } else {
      Serial.print("Error sending data to ThingSpeak. Response code: ");
      Serial.println(response);
    }
  }

  // Wait for 20 seconds before sending the next set of data
  delay(10000);
}










//#include <ESP8266WiFi.h>
//#include <ThingSpeak.h>
//
//const char* WIFI_ID = "RakshitLaptop";
//const char* WIFI_PASSWORD = "12345678";
//
//unsigned long myChannelNumber = 2207955;
//const char* myWriteAPIKey = "7X6XYMLH9HPA8U9Q";
//
//float humidity = 0.0;
//float temperature = 0.0;
//int gasLevel = 0;
//float pressure = 0.0;
//float altitude = 0.0;
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
//String delimit_string(String data, char separator, int index)
//{
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
//}
//
//void loop() {
//  // Assuming you receive the data and store it in the 'data' variable
//  String receivedData = ""; // Example received data string
//
//  // Call the parseData function to extract values and store them in variables
//  parseData(receivedData);
//
//  // Update the fields with the parsed data
//  ThingSpeak.setField(1, String(humidity));
//  ThingSpeak.setField(2, String(temperature));
//  ThingSpeak.setField(3, String(gasLevel));
//  ThingSpeak.setField(4, String(pressure));
//  ThingSpeak.setField(5, String(altitude));
//
//  // Write the data to ThingSpeak
//  int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
//
//  if (response == 200) {
//    Serial.println("Data sent to ThingSpeak successfully");
//    Serial.println(receivedData);
//  } else {
//    Serial.print("Error sending data to ThingSpeak. Response code: ");
//    Serial.println(response);
//  }
//
//  // Wait for 20 seconds before sending the next set of data
//  delay(20000);
//}



//
//
//
//
//#include <ESP8266WiFi.h>
//#include <ThingSpeak.h>
//
//const char* WIFI_ID = "RakshitLaptop";
//const char* WIFI_PASSWORD = "12345678";
//
//unsigned long myChannelNumber = 2207955;
//const char* myWriteAPIKey = "7X6XYMLH9HPA8U9Q";
//
//float humidity = 50.0;
//float temperature = 25.0;
//int gasLevel = 75;
//float pressure = 1013.25;
//float altitude = 50.0;
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
//void loop() {
//  // Update the fields with the static data
//  ThingSpeak.setField(1, String(humidity));
//  ThingSpeak.setField(2, String(temperature));
//  ThingSpeak.setField(3, String(gasLevel));
//  ThingSpeak.setField(4, String(pressure));
//  ThingSpeak.setField(5, String(altitude));
//
//  // Write the data to ThingSpeak
//  int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
//
//  if (response == 200) {
//    Serial.println("Data sent to ThingSpeak successfully");
//  } else {
//    Serial.print("Error sending data to ThingSpeak. Response code: ");
//    Serial.println(response);
//  }
//
//  // Wait for 20 seconds before sending the next set of data
//  delay(20000);
//}
