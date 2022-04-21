#!/bin/bash

~/arduino-cli/arduino-cli compile --verbose --fqbn Seeeduino:samd:seeed_XIAO_m0 serialpassthrough.ino 
~/arduino-cli/arduino-cli upload --port /dev/ttyACM* --verbose --fqbn Seeeduino:samd:seeed_XIAO_m0
