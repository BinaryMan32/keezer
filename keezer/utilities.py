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
import datetime
import logging
import time


def celsius_to_fahrenheit(c):
    """Convert a Celsius temperature to Fahrenheit."""
    return c * 9.0 / 5.0 + 32.0


def fahrenheit_to_celsius(f):
    """Convert a Celsius temperature to Fahrenheit."""
    return (f - 32.0) * 5.0 / 9.0


def setup_logging(log_file_path):
    format = '%(asctime)s %(levelname)s:%(name)s %(message)s'
    formatter = logging.Formatter(format)

    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)

    fileHandler = logging.FileHandler(log_file_path)
    fileHandler.setFormatter(formatter)
    fileHandler.setLevel(logging.INFO)
    logger.addHandler(fileHandler)

    streamHandler = logging.StreamHandler()
    streamHandler.setFormatter(formatter)
    streamHandler.setLevel(logging.DEBUG)
    logger.addHandler(streamHandler)


def sleep_until_next_minute():
    t = datetime.datetime.utcnow()
    sleep_time = 60 - (t.second + 1e-6 * t.microsecond)
    time.sleep(sleep_time)
