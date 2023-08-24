#include <Arduino.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

// Pin configuration for DHT11 sensor
#define DHTPIN 2
#define DHTTYPE DHT11

// WiFi credentials and ThingSpeak API key
#define SECRET_SSID "@prabh"
#define SECRET_PASS "0987654321"
#define SECRET_API_KEY "47JFWCVHFQZCQ8QU"

// Initialize the DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// WiFi credentials for connecting to the network
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// ThingSpeak Channel configuration
unsigned long channelID = 2250596;  //ThingSpeak Channel ID
const char *apiKey = SECRET_API_KEY;

WiFiClient wifiClient;

void setup() {
  Serial.begin(115200); // Start serial communication for debugging
  dht.begin(); // Initialize the DHT sensor
  connectToWiFi(); // Connect to WiFi network
}

void loop() {
  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if sensor readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Initialize ThingSpeak communication
  ThingSpeak.begin(wifiClient);

  // Set temperature and humidity values as fields
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  // Send data to ThingSpeak and get response
  int response = ThingSpeak.writeFields(channelID, apiKey);

  // Check if data was successfully sent
  if (response == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Failed to send data to ThingSpeak");
  }

  delay(60000); // Wait for 1 minute before reading again
}

// Function to connect to WiFi network
void connectToWiFi() {
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    // Attempt to connect to WiFi using provided credentials
    status = WiFi.begin(ssid, pass);
    delay(500);
  }
}
