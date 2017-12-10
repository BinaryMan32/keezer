
import Adafruit_MCP9808.MCP9808 as MCP9808


class Mcp9808:
    def __init__(self, i2c_id):
        self.i2c_id = i2c_id
        self._sensor = MCP9808.MCP9808(address=0x18+self.i2c_id)
        self._sensor.begin()

    def read(self):
        return self._sensor.readTempC()
