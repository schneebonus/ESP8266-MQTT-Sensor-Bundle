#include "MQ135.h"
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Ticker.h>

#define DEBUGGING false

#define MQTT_PUB "sensory/esp"
#define DEVICE_ID 1

#define DHT_TYPE DHT22


const char* ssid = "";          # ToDo
const char* password = "";      # ToDo
const char* mqttServer = "";    # ToDo
const int mqttPort = 1883;      # ToDo
const char* mqttUser = "";      # ToDo
const char* mqttPassword = "";  # ToDo

const int DHT_PIN = 5;
DHT dht(DHT_PIN, DHT_TYPE);
MQ135 gasSensor = MQ135(A0);
WiFiClient espClient;
PubSubClient client(espClient);
volatile int watchdogCount = 0;
Ticker secondTick;

void setup() {
  // put your setup code here, to run once:
  if(DEBUGGING)Serial.begin(115200);
  dht.begin();
  connect_wifi();               // wlan verbinden
  connect_mqtt();               // mqtt verbinden

  // start the watchdog
  // restarts esp in case main loop does not finish after 10 seconds
  secondTick.attach(1, ISRWatchdog);
}

void loop() {
  // reset watchdog to prevent reset
  watchdogCount = 0;

  // handle wifi connection
  int wifi_retry = 0;
  while(WiFi.status() != WL_CONNECTED && wifi_retry < 5 ) {
      // reconnect wifi if not connected
      wifi_retry++;
      WiFi.disconnect();
      WiFi.mode(WIFI_OFF);
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
      delay(1000);
  }
  if(wifi_retry >= 5) {
      // if reconnect failed 5 times -> restart esp
      ESP.restart();
  }

  // handle mqtt connection
  if(!client.connected())ESP.restart();

  // put your main code here, to run repeatedly:
  send_data();
  delay(60*1000);
}

//---------------------- Sensoren --------------------------//

void send_data() {
  StaticJsonDocument<200> doc;

  // Sensoren lesen
  float h = int(dht.readHumidity() * 10.0);
  float t = int(dht.readTemperature() * 10.0);
  float air_quality = int(gasSensor.getPPM());

  // JSON Objekt erstellen
  doc["t"] = t;
  doc["h"] = h;
  doc["a"] = air_quality;

  // JSON zu String umwandeln
  String payload;
  serializeJson(doc, payload);
  
  // Über MQTT raus senden
  String topic = MQTT_PUB + String(DEVICE_ID);
  client.publish(topic.c_str(), payload.c_str());
  if(DEBUGGING)Serial.print("Topic: ");
  if(DEBUGGING)Serial.println(topic);
  if(DEBUGGING)Serial.print("Data: ");
  if(DEBUGGING)Serial.println(payload);
}

//---------------------- WIFI --------------------------//
void connect_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
  }
  if(DEBUGGING)Serial.println("Connected to the WiFi network");
}

//---------------------- MQTT --------------------------//
void connect_mqtt() {
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    if (client.connect(("sensor_esp" + String(DEVICE_ID)).c_str(), mqttUser, mqttPassword )) {
      if(DEBUGGING)Serial.println("Connected to MQTT");
    } else {
      if(DEBUGGING)Serial.print("failed with state ");
      if(DEBUGGING)Serial.print(client.state());
      delay(2000);
    }
  }
}

//---------------------- Watchdog --------------------------//
// watchdog to reset after crashes
void ISRWatchdog(){
  watchdogCount++;
  if(watchdogCount == 10){
    ESP.reset();
  }
}
