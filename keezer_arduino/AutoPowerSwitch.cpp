#include "AutoPowerSwitch.h"

AutoPowerSwitch::AutoPowerSwitch(int pin, power_mode_t initialMode, bool initialAutoState)
:
mPowerSwitch(pin, getDesiredState(initialMode, initialAutoState)),
mMode(initialMode),
mAutoState(initialAutoState),
mObserver(nullptr)
{
}

void AutoPowerSwitch::setObserver(Observer* observer) {
  mObserver = observer;
}

void AutoPowerSwitch::setup() {
  mPowerSwitch.setup();
}

void AutoPowerSwitch::setMode(power_mode_t mode) {
  if (mode != mMode && mObserver != nullptr) {
    mObserver->modeChanged(mMode, mode);
  }
  mMode = mode;
  setState(getDesiredState());
}

power_mode_t AutoPowerSwitch::getMode() const {
  return mMode;
}

void AutoPowerSwitch::setAutoState(bool autoState) {
  if (autoState != mAutoState && mObserver != nullptr) {
    mObserver->autoStateChanged(mAutoState, autoState);
  }
  mAutoState = autoState;
  setState(getDesiredState());
}

bool AutoPowerSwitch::getAutoState() const {
  return mAutoState;
}

bool AutoPowerSwitch::getState() const {
  return mPowerSwitch.getState();
}

void AutoPowerSwitch::setState(bool state) {
  const bool oldState = mPowerSwitch.getState();
  if (state != oldState && mObserver != nullptr) {
    mObserver->stateChanged(oldState, state);
  }
  mPowerSwitch.setState(state);
}

bool AutoPowerSwitch::getDesiredState() const {
  return getDesiredState(mMode, mAutoState);
}

// static
bool AutoPowerSwitch::getDesiredState(power_mode_t mode, bool autoSwitchState) {
  switch(mode) {
    case POWER_MODE_OFF:
      return false;
    case POWER_MODE_ON:
      return true;
    default:
    case POWER_MODE_AUTO:
      return autoSwitchState;
  }
}
