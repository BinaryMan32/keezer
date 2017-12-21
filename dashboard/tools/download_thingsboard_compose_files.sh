#!/bin/bash

# See https://thingsboard.io/docs/user-guide/install/docker/

TAG=v1.3.1
THINGSBOARD_BASE_URL=https://raw.githubusercontent.com/thingsboard/thingsboard/${TAG}
curl -L ${THINGSBOARD_BASE_URL}/docker/docker-compose.yml > docker-compose.yml
curl -L ${THINGSBOARD_BASE_URL}/docker/.env > .env
curl -L ${THINGSBOARD_BASE_URL}/docker/tb.env > tb.env
