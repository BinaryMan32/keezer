#include "temperature.h"

namespace temperature {
  float convert_c_to_f(float c) {
      return c * 9.0f / 5.0f + 32.0f;
  }
  float convert_f_to_c(float f) {
      return (f - 32.0f) * 5.0f / 9.0f;
  }
}
