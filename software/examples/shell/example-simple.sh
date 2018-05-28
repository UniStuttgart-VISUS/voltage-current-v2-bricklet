#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Voltage/Current Bricklet 2.0

# Get current voltage
tinkerforge call voltage-current-v2-bricklet $uid get-voltage

# Get current current
tinkerforge call voltage-current-v2-bricklet $uid get-current
