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

# Register power callback
vc.register_callback(BrickletVoltageCurrentV2::CALLBACK_POWER) do |power|
  puts "power: #{power/1000.0} W"
end

# Configure threshold for power "greater than 10 W"
# with a debounce period of 1s (1000ms)
vc.set_power_callback_configuration 1000, false, '>', 10*1000, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
