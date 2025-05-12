#define BLYNK_TEMPLATE_ID "TMPL3ItcOv5un"
#define BLYNK_TEMPLATE_NAME "temperature and humidity"
#define BLYNK_AUTH_TOKEN "g6gUybmxS-67W9u9BI4SmRW4nsJ0i0CH"
#define BLYNK_PRINT Serial  
 
#include <WiFi.h>  // Include WiFi library for ESP32
#include <BlynkSimpleEsp32.h>  // Include Blynk ESP32 library
#include "DHT.h"  // Include DHT sensor library
 
// Blynk and WiFi credentials
const char auth[] = "g6gUybmxS-67W9u9BI4SmRW4nsJ0i0CH";  // Blynk authentication token
const char ssid[] = "Neha M42";  // WiFi SSID
const char pass[] = "FijiShibuNehaSneha";  // WiFi password
 
// Pin configuration
#define DHTPIN 22  // DHT sensor pin
#define DHTTYPE DHT11  // Type of DHT sensor
 
const int exhaustPin = 23;  // exhaust module pin
 
const int temperatureThreshold = 34;  // humidity level threshold for the exhaust
 
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor
 
bool manualMode = false;  // Flag to track if the fan is in manual mode
 
void setup()
{
  Serial.begin(115200); 
  dht.begin();  
  Blynk.begin(auth, ssid, pass); 
  pinMode(exhaustPin, OUTPUT);  
 
  digitalWrite(exhaustPin, LOW); 
  delay(2000);  
}
 
// Function to handle exhaust control from Blynk app
BLYNK_WRITE(V4)
{
  int exhaustControl = param.asInt(); 
  manualMode = (exhaustControl == 1); 
  if (manualMode)
  {
    digitalWrite(exhaustPin, HIGH); 
  }
  else
  {
    digitalWrite(exhaustPin, LOW); 
  }
 
  Blynk.virtualWrite(V3, exhaustControl);  // Update exhaust status on Blynk
}
 
void loop()
{
  Blynk.run();  // Run Blynk
 
  float humidity = dht.readHumidity();  // Read humidity from DHT sensor
  float temperature = dht.readTemperature();  // Read temperature from DHT sensor
 
  // Check if sensor readings are valid
  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
 
  // Print sensor values to the Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
 
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C ");
 
 
  Serial.println();
 
  // Automatic control logic based on temperature levels
  if (!manualMode)
  {
    if (temperature > temperatureThreshold)
    {
      digitalWrite(exhaustPin, HIGH);  // Activate exhaust if temperature above threshold
      Blynk.virtualWrite(V3, HIGH);  // Update Blynk app
      Blynk.logEvent("temperature_level_alert","Temperature is above the threshold!");
    }
    else
    {
      digitalWrite(exhaustPin, LOW);  // Deactivate exhaust if temperature below threshold
      Blynk.virtualWrite(V3, LOW);  // Update Blynk app
    }
  }
 
  // Send sensor values to Blynk
  Blynk.virtualWrite(V1, temperature);  // Send temperature
  Blynk.virtualWrite(V2, humidity);  // Send humidity
 
  delay(1000);  // Wait for a second before next loop iteration
}