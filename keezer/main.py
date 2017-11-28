#
# Copyright 2017 Fred Fettinger
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
import daemon
import logging
import time

import Adafruit_MCP9808.MCP9808 as MCP9808

from . import utilities

log = logging.getLogger(__name__)


def daemon_main():
  utilities.setup_logging('/var/log/keezer.log')

  log.info('daemon_main')

  sensors = [
    MCP9808.MCP9808(address=0x18+0),
    MCP9808.MCP9808(address=0x18+1),
  ]

  import Adafruit_GPIO.GPIO as GPIO
  gpio = GPIO.get_platform_gpio()
  power_pin = 23 # BCM 23, physical pin 16: https://pinout.xyz/pinout/pin16_gpio23
  gpio.setup(power_pin, GPIO.OUT)
  power = False

  for sensor in sensors:
    sensor.begin()
  for t in range(100):
    for index, sensor in enumerate(sensors):
      log.info('sensor[%d]=%f', index, sensor.readTempC())

    log.info('POWER %s', power and 'ON' or 'OFF')
    gpio.output(power_pin, power)
    power = not power

    time.sleep(10)

def main():
  with daemon.DaemonContext():
    daemon_main()
