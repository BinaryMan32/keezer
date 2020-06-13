#ifndef KEEZER_AUTOPOWERSWITCH_H
#define KEEZER_AUTOPOWERSWITCH_H

#include "PowerSwitch.h"
#include "power_mode.h"

class AutoPowerSwitch {
  public:
    class Observer {
      public:
        virtual ~Observer() = default;
        virtual void modeChanged(power_mode_t oldMode, power_mode_t newMode) = 0;
        virtual void autoStateChanged(bool oldAutoState, bool newAutoState) = 0;
        virtual void stateChanged(bool oldState, bool newState) = 0;
    };

    AutoPowerSwitch(int pin, power_mode_t initialMode, bool initialAutoState);
    void setObserver(Observer* observer);
    void setup();
    void setMode(power_mode_t mode);
    power_mode_t getMode() const;
    void setAutoState(bool autoState);
    bool getAutoState() const;
    bool getState() const;
  private:
    void setState(bool state);
    bool getDesiredState() const;
    static bool getDesiredState(power_mode_t mode, bool autoSwitchState);
    PowerSwitch mPowerSwitch;
    power_mode_t mMode;
    bool mAutoState;
    Observer* mObserver;
};

#endif /* KEEZER_AUTOPOWERSWITCH_H */
