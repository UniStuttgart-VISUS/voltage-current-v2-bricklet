#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletVoltageCurrentV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Voltage/Current Bricklet 2.0

# Callback subroutine for power callback
sub cb_power
{
    my ($power) = @_;

    print "power: " . $power/1000.0 . " W\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $vc = Tinkerforge::BrickletVoltageCurrentV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register power callback to subroutine cb_power
$vc->register_callback($vc->CALLBACK_POWER, 'cb_power');

# Configure threshold for power "greater than 10 W"
# with a debounce period of 1s (1000ms)
$vc->set_power_callback_configuration(1000, 0, '>', 10*1000, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
