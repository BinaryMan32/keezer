#include "PowerSwitch.h"

#include <Arduino.h>

PowerSwitch::PowerSwitch(int pin, bool initialState)
:
mPin(pin),
mState(initialState)
{
}

void PowerSwitch::setup() {
  setPower();
  pinMode(mPin, OUTPUT);
}

void PowerSwitch::setState(bool state) {
  if (state != mState) {
    mState = state;
    setPower();
  }
}

bool PowerSwitch::getState() const {
  return mState;
}

void PowerSwitch::setPower() {
  digitalWrite(mPin, mState ? HIGH : LOW);
}
