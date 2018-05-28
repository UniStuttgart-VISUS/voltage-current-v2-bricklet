#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Voltage/Current Bricklet 2.0

# Handle incoming power callbacks
tinkerforge dispatch voltage-current-v2-bricklet $uid power &

# Configure threshold for power "greater than 10 W"
# with a debounce period of 1s (1000ms)
tinkerforge call voltage-current-v2-bricklet $uid set-power-callback-configuration 1000 false threshold-option-greater 10000 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
