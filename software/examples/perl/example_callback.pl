#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletVoltageCurrentV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Voltage/Current Bricklet 2.0

# Callback subroutine for current callback
sub cb_current
{
    my ($current) = @_;

    print "Current: " . $current/1000.0 . " A\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $vc = Tinkerforge::BrickletVoltageCurrentV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register current callback to subroutine cb_current
$vc->register_callback($vc->CALLBACK_CURRENT, 'cb_current');

# Set period for current callback to 1s (1000ms) without a threshold
$vc->set_current_callback_configuration(1000, 0, 'x', 0, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
