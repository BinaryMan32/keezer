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
import statistics

from . import utilities, sensors, switches

log = logging.getLogger(__name__)


def daemon_main():
    utilities.setup_logging('/var/log/keezer.log')

    log.info('daemon_main')

    sensors_by_name = {
        'temp_top': sensors.temperature.Mcp9808(i2c_id=0),
        'temp_bottom': sensors.temperature.Mcp9808(i2c_id=1),
    }

    # BCM 23, physical pin 16: https://pinout.xyz/pinout/pin16_gpio23
    power_switch = switches.GPIO(name='power', pin=23)

    target_temperature_range = tuple(map(utilities.fahrenheit_to_celsius, (38, 42)))
    log.info('target_temperature_range=%r', target_temperature_range)

    while True:
        sensor_readings = dict([(k, s.read()) for (k, s) in sensors_by_name.items()])
        mean_temp = statistics.mean(sensor_readings.values())
        log.info('sensors=%r mean=%.1f', sensor_readings, mean_temp)

        if mean_temp < target_temperature_range[0]:
            power_switch.state = False
        elif mean_temp > target_temperature_range[1]:
            power_switch.state = True

        utilities.sleep_until_next_minute()


def main():
    with daemon.DaemonContext():
        daemon_main()
