#define BLYNK_TEMPLATE_ID "TMPL3UrmROL9O"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation"
#define BLYNK_AUTH_TOKEN "WpjA4zQq69WxbRdCv3mxlLF-f0mUU0zK"


#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h> 

#define SOIL_MOISTURE_PIN 33   // Analog pin for soil moisture sensor
#define WATER_LEVEL_PIN 34   // Analog pin for soil moisture sensor
#define THRESHOLD_MOISTURE 53 // Adjust this value based on your soil moisture sensor readings
#define PUMP_PIN1 2  // Digital pin for controlling the pump
#define PUMP_PIN2 4
#define PUMP_SWITCH V6  // Virtual pin for controlling the pump manually
#define PUMP_SWITCH2 V3  // Virtual pin for controlling the pump manually

char auth[] = BLYNK_AUTH_TOKEN;  // Replace with your Blynk auth token
char ssid[] = "Neha M42";   // Replace with your WiFi credentials
char pass[] = "FijiShibuNehaSneha";
 
BlynkTimer timer;

bool isPumpOn = false;
bool isPumpOn2 = false;  // Variable to track pump status

void sendSensorData()
{
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  Serial.print("Soil Moisture ");
  Serial.println(soilMoisture);
  // Map the analog sensor values to a percentage (0-100) 
  int soilMoisturePercentage = map(soilMoisture, 1072, 2655, 100, 0);
  Serial.print("Soil Moisture Percentage ");
  Serial.println(soilMoisturePercentage);
  // Send soil moisture data to Blynk
  Blynk.virtualWrite(V5, soilMoisturePercentage);

  // Check if the pump should be on based on manual switch or soil moisture level
  if (isPumpOn || soilMoisturePercentage < THRESHOLD_MOISTURE)
  {
    // Turn on the pump
    digitalWrite(PUMP_PIN1, HIGH);
    // Check if the pump is turned on automatically (not manually)
    if (!isPumpOn) {
      // Send alert notification to Blynk app if the pump is turned on automatically
      Blynk.logEvent("moisture_alert","Soil moisture is below the threshold!");
      Serial.println("Soil moisture is below the threshold!");
    }
  }
  else
  {
    // Turn off the pump only if it was not turned on manually
    if (!isPumpOn) {
      digitalWrite(PUMP_PIN1, LOW);
    }
  }
}

void sendSensorData2()
{
  int waterLevel = analogRead(WATER_LEVEL_PIN);
  Serial.print("Water Level ");
  Serial.println(waterLevel);
  // Map the analog sensor values to a percentage (0-100) 
  int waterLevelPercentage = map(waterLevel, 1900, 1200, 100, 0);
  Serial.print("Water Level Percentage ");
  Serial.println(waterLevelPercentage);
  // Send soil moisture data to Blynk
  Blynk.virtualWrite(V1, waterLevelPercentage);

  // Check if the pump should be on based on manual switch or soil moisture level
  if (isPumpOn2 || waterLevelPercentage < THRESHOLD_MOISTURE)
  {
    // Turn on the pump
    digitalWrite(PUMP_PIN2, HIGH);
    // Check if the pump is turned on automatically (not manually)
    if (!isPumpOn2) {
      // Send alert notification to Blynk app if the pump is turned on automatically
      Blynk.logEvent("water_level_alert","Water level is below the threshold!");
      Serial.println("Water level is below the threshold!");
    }
  }
  else
  {
    // Turn off the pump only if it was not turned on manually
    if (!isPumpOn2) {
      digitalWrite(PUMP_PIN2, LOW);
    }
  }
}


BLYNK_WRITE(PUMP_SWITCH)
{
  isPumpOn = param.asInt();
  if (isPumpOn) {
    Serial.println("Pump manually turned ON");
  } else {
    Serial.println("Pump manually turned OFF");
  }
}

BLYNK_WRITE(PUMP_SWITCH2)
{
  isPumpOn2 = param.asInt();
  if (isPumpOn2) {
    Serial.println("Pump manually turned ON");
  } else {
    Serial.println("Pump manually turned OFF");
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(PUMP_PIN1, OUTPUT); // Set pump pin as an output
  pinMode(PUMP_PIN2, OUTPUT);

  Blynk.begin(auth, ssid, pass);

  timer.setInterval(3000L, sendSensorData); // Set the interval for checking soil moisture (every 3 seconds)
  timer.setInterval(3000L, sendSensorData2);

  // Setup switch widget
  Blynk.virtualWrite(PUMP_SWITCH, isPumpOn);
  Blynk.virtualWrite(PUMP_SWITCH2, isPumpOn2);

  Blynk.syncVirtual(PUMP_SWITCH);
  Blynk.syncVirtual(PUMP_SWITCH2);
}

void loop()
{
  Blynk.run();
  timer.run();
}
