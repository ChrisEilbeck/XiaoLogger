#!/bin/bash

BOARD=arduino:mbed_rp2040:pico
PORT=/dev/ttyACM0

arduino-cli compile --upload --verbose --fqbn ${BOARD} --port ${PORT} XiaoLogger.ino
