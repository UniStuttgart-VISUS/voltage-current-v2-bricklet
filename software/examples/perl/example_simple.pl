#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletVoltageCurrentV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Voltage/Current Bricklet 2.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $vc = Tinkerforge::BrickletVoltageCurrentV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current voltage
my $voltage = $vc->get_voltage();
print "Voltage: " . $voltage/1000.0 . " V\n";

# Get current current
my $current = $vc->get_current();
print "Current: " . $current/1000.0 . " A\n";

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
