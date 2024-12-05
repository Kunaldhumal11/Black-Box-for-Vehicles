#include <ESP8266WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>

// Replace with your network credentials
const char* ssid = "";
const char* password = "";

// ThingSpeak API details
unsigned long myChannelNumber = 2476652;
const char* myWriteAPIKey = "FPP7KEL4Y2F7RBLH";

WiFiClient client;

// DHT11 setup
#define DHTPIN D4    // Pin connected to DHT11 data pin
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Vibration sensor setup
#define VIBRATION_PIN D3 // Pin connected to vibration sensor

void setup() {
    Serial.begin(115200);
    dht.begin();
    pinMode(VIBRATION_PIN, INPUT);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Initialize ThingSpeak
    ThingSpeak.begin(client);
}

void loop() {
    // Read data from DHT11 sensor
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Check if any reads failed
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Read data from vibration sensor
    int vibrationValue = digitalRead(VIBRATION_PIN);

    // Print values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%, Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Vibration: ");
    Serial.println(vibrationValue);

    // Update ThingSpeak channel
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);
    ThingSpeak.setField(3, vibrationValue);

    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200) {
        Serial.println("Channel update successful.");
    } else {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
    }

    // Wait 20 seconds before the next update
    delay(20000);
}
