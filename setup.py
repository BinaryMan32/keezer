#!/usr/bin/env python3
from setuptools import setup, find_packages

setup(
  name='Keezer',
  version='1.0',
  description='Keezer Monitor',
  author='Fred Fettinger',
  author_email='fettinge@gmail.com',
  packages=find_packages(exclude=["tests.*", "tests"]),
  entry_points={
    'console_scripts': [
      'keezer=keezer.main:main',
      'keezer_interactive=keezer.main:daemon_main',
    ],
  },
  install_requires=['Adafruit_MCP9808', 'python-daemon'],
)
