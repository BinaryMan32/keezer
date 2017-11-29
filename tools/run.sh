#!/bin/bash
# From https://blog.hypriot.com/post/lets-get-physical/
docker run --interactive --tty \
  --cap-add SYS_RAWIO --device /dev/mem --device /dev/i2c-1 \
  keezer /usr/local/bin/keezer_interactive

