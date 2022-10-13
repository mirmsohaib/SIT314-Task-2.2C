#include "DHTesp.h"
#include <WiFi.h>
#include "ThingSpeak.h"

// Input 
const int DHT_PIN = 15;
DHTesp dhtSensor;

// ThingSpeak
unsigned long myChannelNumber = 1843837;
String apiKey = "150INT0DTLRAZ4UF";
const char* server = "api.thingspeak.com";

// Wifi
WiFiClient client;

void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  WiFiClient  client;
  ThingSpeak.begin(client);
}

void loop() {
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();

  // Data 
  float t = data.temperature;
  float h = data.humidity;

// Http Post Method
// Thingspeak
   if(client.connect(server,80)){

     // String
     String postStr = apiKey;
       postStr +="&field1=";
       postStr += String(t);
       postStr +="&field2=";
       postStr += String(h);
       postStr += "\r\n\r\n";
 
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
      //Serial.println(postStr);

      Serial.println("Temp: " + String(data.temperature, 2) + "°C");
      Serial.println("Humidity: " + String(data.humidity, 1) + "%");
      Serial.println("---");
     }

     delay(1000);
}