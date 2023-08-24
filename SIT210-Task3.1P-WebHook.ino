#include <Arduino.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHTPIN 2      // DHT11 sensor connected to digital pin 2
#define DHTTYPE DHT11 // DHT type

#define SECRET_SSID "@prabh"
#define SECRET_PASS "0987654321"
#define SECRET_API_KEY "47JFWCVHFQZCQ8QU"

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
unsigned long channelID = 2250596; // Replace with your ThingSpeak Channel ID
const char *apiKey = SECRET_API_KEY;

WiFiClient wifiClient;

void setup() {
  Serial.begin(115200);
  dht.begin();
  connectToWiFi();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  ThingSpeak.begin(wifiClient);
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  int response = ThingSpeak.writeFields(channelID, apiKey);

  if (response == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Failed to send data to ThingSpeak");
  }

  delay(60000); // Wait for 1 minute before reading again
}

void connectToWiFi() {
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.print("Connecting to WiFi...");
    status = WiFi.begin(ssid, pass);
    delay(500);
  }
  Serial.println();
  Serial.println("Connected to WiFi");
}
