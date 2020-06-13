# Keezer Arduino

Controls power of a freezer to maintain the temperature of the kegs inside.

## Configuration

1. `keezer_arduino.ino`: set mosquitto broker host name
   ```cpp
   const char* mqtt_host = "minerva";
   ```
2. `wifi.cpp`: configure WiFi credentials
   ```cpp
   const char* wifi_ssid = "your_ssid";
   const char* wifi_pass = "your_password";
   ```

## Pin connections

See https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/ for a detailed description of
which pins should be used on the ESP8266.

## Mosquitto

### Output Messages

* `keezer/power/state`: Whether the keezer is powered, either `0` or `1`
* `keezer/power/desired_state`: Whether the keezer would be powered based on temperature sensors.
  If `keezer/power/mode` is `AUTO`, this should match `keezer/power/state`.
* `keezer/temp/top`/`keezer/temp/bottom`: the temperature in degrees Celsius read by each of the
  sensors. Example: `22.06`

### Input Messages

Use `keezer/power/mode` to control the power behavior as follows.

* `OFF`: power always off. Execute using:
  ```
  mosquitto_pub -t keezer/power/mode -m OFF
  ```
* `ON`: power always on. Execute using:
  ```
  mosquitto_pub -t keezer/power/mode -m ON
  ```
* `AUTO`: power controlled by temperature thresholds. Execute using:
  ```
  mosquitto_pub -t keezer/power/mode -m AUTO
  ```

### Monitoring

Monitor the behavior using the following command:
```
mosquitto_sub -v -t keezer/#
```