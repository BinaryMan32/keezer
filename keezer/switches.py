import logging

import Adafruit_GPIO.GPIO

__all__ = ['GPIO']

log = logging.getLogger(__name__)

switch_state_str = {
    True: 'ON',
    False: 'OFF',
}


class GPIO:
    _platform_gpio = Adafruit_GPIO.GPIO.get_platform_gpio()

    def __init__(self, name, pin, state=False):
        self.name = name
        self.pin = pin
        self._platform_gpio.setup(self.pin, Adafruit_GPIO.GPIO.OUT)
        self._set_state(state)

    @property
    def state(self):
        return self._state

    @state.setter
    def state(self, state):
        if state != self._state:
            log.info('state change: %(switch_name)s %(old_state)s -> %(new_state)s',
                     dict(switch_name=self.name,
                          old_state=switch_state_str[self._state],
                          new_state=switch_state_str[state]))
            self._set_state(state)

    def _set_state(self, state):
        self._platform_gpio.output(self.pin, state)
        self._state = state
