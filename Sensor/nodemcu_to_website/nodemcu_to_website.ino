#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// Replace with your network credentials
const char* ssid = "RakshitLaptop";
const char* password = "12345678";

// Firebase configuration
#define FIREBASE_HOST "https://blazealert-ac27a-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "AIzaSyD_PRlvDA3Oy3Lh4b6Ku2Joa2ivGdhVZvY"

// Initialize Firebase
FirebaseData firebaseData;
FirebaseJson json;

void setup() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  
  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  // Enable serial communication
  Serial.begin(9600);
}

void loop() {
  // Receive data from Arduino over Serial
  while (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    Serial.println(receivedData);

    // Parse received sensor data
    float temperature, humidity, gasLevel, pressure, latitude, longitude;
    sscanf(receivedData.c_str(), "Temperature: %f, Humidity: %f, Gas Level: %f, Pressure: %f, Latitude: %f, Longitude: %f", &temperature, &humidity, &gasLevel, &pressure, &latitude, &longitude);
    
    // Store sensor data in Firebase
    json.set("temperature", temperature);
    json.set("humidity", humidity);
    json.set("gasLevel", gasLevel);
    json.set("pressure", pressure);
    json.set("latitude", latitude);
    json.set("longitude", longitude);
    
    if (Firebase.updateNode(firebaseData, "/sensorData", json)) {
      Serial.println("Sensor data stored in Firebase");
    } else {
      Serial.println("Error storing sensor data in Firebase");
      Serial.println(firebaseData.errorReason());
    }
  }
  
  delay(1000);  // Adjust delay as per your requirements
}

// Function to generate HTML code with sensor data
String generateHTML(float temperature, float humidity, float gasLevel, float pressure, float latitude, float longitude) {
  String htmlCode = "<html><body>";
  htmlCode += "<h1>Sensor Data</h1>";
  htmlCode += "<p>Temperature: " + String(temperature) + " C</p>";
  htmlCode += "<p>Humidity: " + String(humidity) + " %</p>";
  htmlCode += "<p>Gas Level: " + String(gasLevel) + "</p>";
  htmlCode += "<p>Pressure: " + String(pressure) + " Pa</p>";
  htmlCode += "<p>Latitude: " + String(latitude) + "</p>";
  htmlCode += "<p>Longitude: " + String(longitude) + "</p>";
  htmlCode += "</body></html>";
  
  return htmlCode;
}
