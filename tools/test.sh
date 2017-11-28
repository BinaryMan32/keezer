#!/bin/bash
# From https://thekev.in/blog/2016-11-18-python-in-docker/
docker run --rm --interactive --tty \
  --volume $(pwd):/src \
  keezer
