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
import json
import logging
import urllib.request

log = logging.getLogger(__name__)


class Client:

    def __init__(self, url):
        self.url = url

    def update_device(self, token, path, data):
        device_url = self.url + '/{}/{}'.format(token, path)
        data_json = json.dumps(data).encode('utf8')
        log.debug('%s: request: %s', path, data_json)
        headers = {'content-type': 'application/json'}
        request = urllib.request.Request(device_url,
                                         data=data_json,
                                         headers=headers)
        response = urllib.request.urlopen(request)
        log.debug('%s: response: %s', path, response.read().decode('utf8'))

    def update_attributes(self, token, attributes):
        """See https://thingsboard.io/docs/reference/http-api/#publish-attribute-update-to-the-server"""
        self.update_device(token, 'attributes', attributes)

    def update_telemetry(self, token, values, timestamp=None):
        """See https://thingsboard.io/docs/reference/http-api/#telemetry-upload-api"""
        if timestamp is None:
            timestamp = time.time()
        timestamp = int(round(1000 * timestamp))
        data = {
            'ts': timestamp,
            'values': values,
        }
        self.update_device(token, 'telemetry', data)
