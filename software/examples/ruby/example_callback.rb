#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_voltage_current_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Voltage/Current Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
vc = BrickletVoltageCurrentV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register current callback
vc.register_callback(BrickletVoltageCurrentV2::CALLBACK_CURRENT) do |current|
  puts "Current: #{current/1000.0} A"
end

# Set period for current callback to 1s (1000ms) without a threshold
vc.set_current_callback_configuration 1000, false, 'x', 0, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
