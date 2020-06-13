#include "Adafruit_MCP9808.h"
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include "AutoPowerSwitch.h"
#include "power_mode.h"
#include "temperature.h"
#include "wifi.h"

// replace MQTT_HOST with a quoted mosquitto broker host name, for example: "hostname"
const char* mqtt_host = MQTT_HOST;
const int   mqtt_port = 1883; // use 8883 for SSL

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, mqtt_host, mqtt_port);

struct TEMP_SENSOR {
  const char* name;
  uint8_t addr;
  Adafruit_MCP9808 sensor;
  Adafruit_MQTT_Publish mqtt;
};

static const uint8_t kNumSensors = 2;
TEMP_SENSOR temp_sensors[kNumSensors] = {
  {
    .name = "top",
    .addr = 0x18,
    .sensor = Adafruit_MCP9808(),
    .mqtt = Adafruit_MQTT_Publish(&mqtt, "keezer/temp/top")
  },
  {
    .name = "bottom",
    .addr = 0x19,
    .sensor = Adafruit_MCP9808(),
    .mqtt = Adafruit_MQTT_Publish(&mqtt, "keezer/temp/bottom")
  }
};

AutoPowerSwitch powerSwitch(13, POWER_MODE_AUTO, false);

class PowerSwitchObserver : public AutoPowerSwitch::Observer {
  public:
    PowerSwitchObserver()
    :
    publishState(&mqtt, "keezer/power/state"),
    publishAutoState(&mqtt, "keezer/power/desired_state")
    {
    }
  protected:
    void modeChanged(power_mode_t oldMode, power_mode_t newMode) override {
      Serial.print("PowerSwitch: modeChanged: ");
      Serial.print(get_power_mode_string(oldMode));
      Serial.print(" -> ");
      Serial.println(get_power_mode_string(newMode));
    }
    void autoStateChanged(bool oldAutoState, bool newAutoState) override {
      Serial.print("PowerSwitch: autoStateChanged: ");
      Serial.print(get_bool_string(oldAutoState));
      Serial.print(" -> ");
      Serial.println(get_bool_string(newAutoState));
      publishAutoState.publish(static_cast<uint32_t>(newAutoState));
    }
    void stateChanged(bool oldState, bool newState) override {
      Serial.print("PowerSwitch: stateChanged: ");
      Serial.print(get_bool_string(oldState));
      Serial.print(" -> ");
      Serial.println(get_bool_string(newState));
      publishState.publish(static_cast<uint32_t>(newState));
    }
  private:
    Adafruit_MQTT_Publish publishState;
    Adafruit_MQTT_Publish publishAutoState;
};
PowerSwitchObserver powerSwitchObserver;

Adafruit_MQTT_Subscribe mqtt_power_mode = Adafruit_MQTT_Subscribe(&mqtt, "keezer/power/mode");

void set_power_mode_callback(char *data, uint16_t len) {
  power_mode_t power_mode = powerSwitch.getMode();
  if (get_power_mode_from_string(data, len, power_mode)) {
    powerSwitch.setMode(power_mode);
  } else {
    Serial.println("Error parsing power mode");
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Keezer controller");

  powerSwitch.setObserver(&powerSwitchObserver);
  powerSwitch.setup();

  // enable temp sensors
  for (uint8_t i = 0; i < kNumSensors; ++i) {
    if (!temp_sensors[i].sensor.begin(temp_sensors[i].addr)) {
      Serial.print("Couldn't find MCP9808 ");
      Serial.println(temp_sensors[i].name);
    }
  }

  wifi_setup();

  mqtt_power_mode.setCallback(set_power_mode_callback);
  mqtt.subscribe(&mqtt_power_mode);
}

bool mqtt_loop() {
  // Stop if already connected.
  if (mqtt.connected()) {
    return true;
  }

  Serial.print("Connecting to MQTT... ");
  int8_t ret = mqtt.connect();
  bool is_connected = ret == 0;
  if (is_connected) {
    Serial.println("Connected!");
  } else {
    Serial.println(mqtt.connectErrorString(ret));
  }
  return is_connected;
}

const float target_temperature_range_c_min = temperature::convert_f_to_c(38);
const float target_temperature_range_c_max = temperature::convert_f_to_c(42);

void loop() {
  // Ensure the connection to the MQTT server is alive
  bool is_connected = wifi_loop() && mqtt_loop();

  // read temperatures
  float temp_celsius[kNumSensors];
  Serial.print("temp:");
  for (uint8_t i = 0; i < kNumSensors; ++i) {
    temp_sensors[i].sensor.wake();
    temp_celsius[i] = temp_sensors[i].sensor.readTempC();
    temp_sensors[i].sensor.shutdown_wake(1);
    temp_sensors[i].mqtt.publish(temp_celsius[i]);
    Serial.print(' ');
    Serial.print(temp_sensors[i].name);
    Serial.print(": ");
    Serial.print(temp_celsius[i], 4);
    Serial.print("*C");
  }

  float mean_temperature_c = 0;
  for (uint8_t i = 0; i < kNumSensors; ++i) {
    if (!isnan(temp_celsius[i])) {
      mean_temperature_c += 0.5f * temp_celsius[i];
    }
  }
  Serial.print(" mean: ");
  Serial.print(mean_temperature_c, 4);
  Serial.print("*C");
  Serial.println();

  // determine power setting if in AUTO mode
  if (powerSwitch.getAutoState()) {
    if (mean_temperature_c < target_temperature_range_c_min) {
      powerSwitch.setAutoState(false);
    }
  } else {
    if (mean_temperature_c > target_temperature_range_c_max) {
      powerSwitch.setAutoState(true);
    }
  }

  // wait for commands
  const int delay_msec = 60 * 1000;
  if (is_connected) {
    mqtt.processPackets(delay_msec);
  } else {
    delay(delay_msec);
  }
}
