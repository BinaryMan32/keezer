#!/bin/bash
docker run --detach \
  --cap-add SYS_RAWIO --device /dev/mem --device /dev/i2c-1 \
  keezer
