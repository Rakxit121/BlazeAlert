#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

const char* WIFI_ID = "RakshitLaptop";
const char* WIFI_PASSWORD = "12345678";

unsigned long myChannelNumber = 2207955;
const char* myWriteAPIKey = "7X6XYMLH9HPA8U9Q";

float humidity = 50.0;
float temperature = 25.0;
int gasLevel = 75;
float pressure = 1013.25;
float altitude = 50.0;

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

void loop() {
  // Update the fields with the static data
  ThingSpeak.setField(1, String(humidity));
  ThingSpeak.setField(2, String(temperature));
  ThingSpeak.setField(3, String(gasLevel));
  ThingSpeak.setField(4, String(pressure));
  ThingSpeak.setField(5, String(altitude));

  // Write the data to ThingSpeak
  int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (response == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
  } else {
    Serial.print("Error sending data to ThingSpeak. Response code: ");
    Serial.println(response);
  }

  // Wait for 20 seconds before sending the next set of data
  delay(20000);
}
