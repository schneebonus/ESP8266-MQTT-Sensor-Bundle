# ESP8266-MQTT-Sensor-Bundle

MQTT firmware for ESP8266 + DHT22 (temperature + humidity) + MQ-135 (air quality).

![esp8266 sensor bundle](https://github.com/schneebonus/ESP8266-MQTT-Sensor-Bundle/blob/main/esp_sensor_bundle.jpeg?raw=true)

### Requirenments

- ESP8266 board definition ( https://arduino-esp8266.readthedocs.io/en/latest/installing.html )
- PubSubClient.h ( https://pubsubclient.knolleary.net/ )
- RCSwitch.h ( https://github.com/sui77/rc-switch )
- ESP8266WiFi.h ( https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi )
- Ticker.h ( https://github.com/esp8266/Arduino/tree/master/libraries/Ticker )
- MQ135.h ( https://github.com/Phoenix1747/MQ135 )
- ArduinoJson.h https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

### Install

### Node-RED integration

Use nodered.js as shown here:

![esp8266 sensor bundle nodered](https://github.com/schneebonus/ESP8266-MQTT-Sensor-Bundle/blob/main/sensor_bundle_nodered.png?raw=true=)

### Grafana air quality

Example value mapping for air quality:

![grafana value mapping](https://github.com/schneebonus/ESP8266-MQTT-Sensor-Bundle/blob/main/grafana_air_quality.png?raw=true)

### Credits

- https://smarthome-blogger.de/blog/esp8266-projekte/esp8266-dht22-temperatur-sensor
- https://how2electronics.com/iot-air-quality-index-monitoring-esp8266/
