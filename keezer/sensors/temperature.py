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
import Adafruit_MCP9808.MCP9808 as MCP9808


class Mcp9808:
    def __init__(self, i2c_id):
        self.i2c_id = i2c_id
        self._sensor = MCP9808.MCP9808(address=0x18+self.i2c_id)
        self._sensor.begin()

    def read(self):
        return self._sensor.readTempC()
