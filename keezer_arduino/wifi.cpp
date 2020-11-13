#include <ESP8266WiFi.h>

// replace the following constants with quoted strings
const char* wifi_ssid = SET_SSID_STRING;
const char* wifi_pass = SET_PASSWORD_STRING;

void wifi_setup() {
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("keezer");
  WiFi.begin(wifi_ssid, wifi_pass);
}

wl_status_t wifi_status_prev = WL_NO_SHIELD;

bool wifi_loop() {
  wl_status_t wifi_status = WiFi.status();
  if (wifi_status != wifi_status_prev) {
    Serial.print("wifi_status: ");
    Serial.println(wifi_status);
    wifi_status_prev = wifi_status;
  }
  return wifi_status == WL_CONNECTED;
}
