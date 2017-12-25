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
import signal
import statistics
import time

from . import utilities, sensors, switches, thingsboard

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

    # Save default signal handlers for chaining
    default_signal_handlers = {}

    def switches_off(signum, frame):
        """Signal handler to turn switches off on exit."""
        log.warning('signal handler called, signum=%r', signum)
        power_switch.state = False
        default_signal_handlers[signum](signum, frame)
    for signum in (signal.SIGINT, signal.SIGTERM):
        default_signal_handlers[signum] = signal.signal(signum, switches_off)

    target_temperature_range = tuple(map(utilities.fahrenheit_to_celsius, (38, 42)))
    log.info('target_temperature_range=%r', target_temperature_range)

    thingsboard_client = thingsboard.Client('http://tb:8080/api/v1')

    while True:
        timestamp = time.time()
        sensor_readings = dict([(k, s.read()) for (k, s) in sensors_by_name.items()])
        mean_temp = statistics.mean(sensor_readings.values())
        log.info('sensors=%r mean=%.1f', sensor_readings, mean_temp)

        if mean_temp < target_temperature_range[0]:
            power_switch.state = False
        elif mean_temp > target_temperature_range[1]:
            power_switch.state = True

        telemetry = dict(sensor_readings.items())
        telemetry.update(power=(1 if power_switch.state else 0))
        thingsboard_client.update_telemetry(token='5FD8mwCgaI2gcXz0Jlif',
                                            values=telemetry,
                                            timestamp=timestamp)

        utilities.sleep_until_next_minute()


def main():
    with daemon.DaemonContext():
        daemon_main()
