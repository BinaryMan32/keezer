#ifndef KEEZER_POWER_MODE_H
#define KEEZER_POWER_MODE_H

#include <stdint.h>

enum power_mode_t {
  POWER_MODE_OFF = 0,
  POWER_MODE_ON = 1,
  POWER_MODE_AUTO = 2,
};

const char* get_bool_string(bool state);

const char* get_power_mode_string(power_mode_t power_mode);

bool get_power_mode_from_string(char *data, uint16_t len, power_mode_t& out_power_mode);

#endif /* KEEZER_POWER_MODE_H */
