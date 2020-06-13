#include "power_mode.h"

#include <string.h>

struct power_mode_info {
  const char* name;
  power_mode_t value;
};

static const uint8_t kNumPowerModes = 3;
const power_mode_info power_modes[kNumPowerModes] = {
  {.name = "OFF", .value = POWER_MODE_OFF},
  {.name = "ON", .value = POWER_MODE_ON},
  {.name = "AUTO", .value = POWER_MODE_AUTO},
};

const char* get_bool_string(bool state) {
    return state ? "ON" : "OFF";
}

const char* get_power_mode_string(power_mode_t power_mode) {
  return power_modes[power_mode].name;
}

bool get_power_mode_from_string(char *data, uint16_t len, power_mode_t& out_power_mode) {
  for (uint8_t i = 0; i < kNumPowerModes; ++i) {
    if (strncmp(power_modes[i].name, data, len) == 0 && power_modes[i].name[len] == 0) {
      out_power_mode = power_modes[i].value;
      return true;
    }
  }
  return false;
}
