#include <DHT.h>  // Include the DHT library to work with the DHT11 sensor

// Define pins and sensor type
#define LED_PIN 2       // LED connected to pin 2 of ESP32

#define DHT_PIN 14      // DHT11 sensor connected to pin 14 of ESP32
#define DHT_TYPE DHT11  // Specify DHT11 as the type of sensor

// Create a DHT object to interact with the sensor
DHT dht(DHT_PIN, DHT_TYPE);

// Variables for monitoring state and threshold settings
bool monitoring = true;           // Boolean to control whether monitoring is active
float tempThresholdHigh = 30;     // Default high temperature threshold (in Celsius)
float tempThresholdLow = 15;      // Default low temperature threshold (in Celsius)
float humidityThresholdHigh = 70; // Default high humidity threshold (in percentage)
float humidityThresholdLow = 30;  // Default low humidity threshold (in percentage)

void setup() {
  pinMode(LED_PIN, OUTPUT);        // Set LED pin as output
  digitalWrite(LED_PIN, LOW);      // Turn LED off initially

  Serial.begin(9600);              // Start serial communication at 9600 baud
  dht.begin();                     // Initialize the DHT11 sensor

  displayMenu();                   // Show the user menu on the serial monitor
}

void loop() {
  // Check if there is any input from the user in the serial monitor
  if (Serial.available() > 0) {
    handleSerialInput();  // Process user commands
  }

  // If monitoring is active, read and display sensor data
  if (monitoring) {
    float temperature = dht.readTemperature();  // Read temperature in Celsius
    float humidity = dht.readHumidity();        // Read humidity in percentage

    // Print temperature and humidity values to the serial monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Control the LED based on temperature and humidity thresholds
    if (temperature > tempThresholdHigh || humidity > humidityThresholdHigh) {
      digitalWrite(LED_PIN, HIGH);  // Turn on LED if thresholds are exceeded
    } else if (temperature < tempThresholdLow || humidity < humidityThresholdLow) {
      blinkLED(1000);  // Blink LED if values are below low thresholds (1 second interval)
    } else {
      digitalWrite(LED_PIN, LOW);   // Turn off LED if values are within normal range
    }
  }

  delay(2000);  // Wait for 2 seconds before reading sensor data again
}

// Function to blink the LED with a specified interval
void blinkLED(int interval) {
  digitalWrite(LED_PIN, HIGH);  // Turn on LED
  delay(interval / 2);          // Wait for half the interval
  digitalWrite(LED_PIN, LOW);   // Turn off LED
  delay(interval / 2);          // Wait for the other half of the interval
}

// Function to handle user input from the serial monitor
void handleSerialInput() {
  String input = Serial.readStringUntil('\n');  // Read user input until newline character
  input.trim();  // Remove any whitespace or newlines from the input

  // Handle different commands entered by the user
  if (input == "start") {
    monitoring = true;  // Start monitoring
    Serial.println("Monitoring started.");
  } else if (input == "stop") {
    monitoring = false;            // Stop monitoring
    digitalWrite(LED_PIN, LOW);     // Turn off LED when monitoring is stopped
    Serial.println("Monitoring stopped.");
  } 
  // Set high temperature threshold
  else if (input.startsWith("setTempHigh ")) {
    tempThresholdHigh = input.substring(12).toFloat();  // Extract and set the new high temperature threshold
    Serial.print("High temperature threshold set to: ");
    Serial.println(tempThresholdHigh);
  } 
  // Set low temperature threshold
  else if (input.startsWith("setTempLow ")) {
    tempThresholdLow = input.substring(11).toFloat();   // Extract and set the new low temperature threshold
    Serial.print("Low temperature threshold set to: ");
    Serial.println(tempThresholdLow);
  } 
  // Set high humidity threshold
  else if (input.startsWith("setHumidityHigh ")) {
    humidityThresholdHigh = input.substring(16).toFloat();  // Extract and set the new high humidity threshold
    Serial.print("High humidity threshold set to: ");
    Serial.println(humidityThresholdHigh);
  } 
  // Set low humidity threshold
  else if (input.startsWith("setHumidityLow ")) {
    humidityThresholdLow = input.substring(15).toFloat();   // Extract and set the new low humidity threshold
    Serial.print("Low humidity threshold set to: ");
    Serial.println(humidityThresholdLow);
  } else {
    Serial.println("Unknown command.");  // Display error if command is not recognized
  }

  displayMenu();  // Redisplay the menu after each command
}

// Function to display a menu of available commands in the serial monitor
void displayMenu() {
  Serial.println();
  Serial.println("Menu:");
  Serial.println("start - Start Monitoring");
  Serial.println("stop - Stop Monitoring");
  Serial.println("setTempHigh <value> - Set high temperature threshold");
  Serial.println("setTempLow <value> - Set low temperature threshold");
  Serial.println("setHumidityHigh <value> - Set high humidity threshold");
  Serial.println("setHumidityLow <value> - Set low humidity threshold");
  Serial.println();
}
