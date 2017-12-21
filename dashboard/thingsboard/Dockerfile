#
# Copyright © 2016-2017 The Thingsboard Authors
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

# Created based on thingsboard Dockerfile, with changes to build on a
# Raspberry Pi. See https://thingsboard.io/docs/user-guide/install/rpi/

FROM oysteinjakobsen/armv7-oracle-java8

RUN apt-get update \
        && apt-get install -y ca-certificates \
        && rm -rf /var/lib/apt/lists/*

RUN wget https://raw.githubusercontent.com/thingsboard/thingsboard/v1.3.1/docker/tb/run-application.sh \
         -O /run-application.sh

RUN wget https://github.com/thingsboard/thingsboard/releases/download/v1.3.1/thingsboard-1.3.1.deb \
         -O /thingsboard.deb

RUN apt-get update \
        && apt-get install -y nmap \
        && chmod +x /run-application.sh \
        && rm -rf /var/lib/apt/lists/*

ENV JAVA_OPTS="platform=rpi -Xms256M -Xmx256M"
