#ifndef KEEZER_POWERSWITCH_H
#define KEEZER_POWERSWITCH_H

class PowerSwitch {
  public:
    PowerSwitch(int pin, bool initialState);
    void setup();
    void setState(bool state);
    bool getState() const;
  private:
    void setPower();
    int mPin;
    bool mState;

};

#endif /* KEEZER_POWERSWITCH_H */
