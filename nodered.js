/*
Erwartete msg:
msg.topic: sensory/devicename
msg.payload: {"t":int * 10,"h":int * 10,"a":int}
*/

let sensory = {
    "esp1" : "Schreibtisch",
    "esp2" : "Badezimmer",
    "esp3" : "Kammer",
    };

sensor_name = sensory[(msg.topic.split("/"))[1]];
parsed = JSON.parse(msg.payload);

// für influxdb aufbereiten
msg.payload = {
      bucket: 'sensors',
    
      precision: 'ms',
    
      data: [
        {
          measurement: 'Sensor Bundle',
    
          tags: {
            deviceId: sensor_name
          },
    
          fields: {
            temperature: +(parsed.t) / 10.0,
            humidity: +(parsed.h) / 10.0,
            "air_quality": +(parsed.a),
          },
    
          timestamp: Date.now()
        },
      ]
    };

return msg;
