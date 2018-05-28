#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Voltage/Current Bricklet 2.0

# Handle incoming current callbacks
tinkerforge dispatch voltage-current-v2-bricklet $uid current &

# Set period for current callback to 1s (1000ms) without a threshold
tinkerforge call voltage-current-v2-bricklet $uid set-current-callback-configuration 1000 false threshold-option-off 0 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
