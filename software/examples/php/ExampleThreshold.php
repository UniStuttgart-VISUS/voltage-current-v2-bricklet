<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletVoltageCurrentV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletVoltageCurrentV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Voltage/Current Bricklet 2.0

// Callback function for power callback
function cb_power($power)
{
    echo "power: " . $power/1000.0 . " W\n";
}

$ipcon = new IPConnection(); // Create IP connection
$vc = new BrickletVoltageCurrentV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register power callback to function cb_power
$vc->registerCallback(BrickletVoltageCurrentV2::CALLBACK_POWER, 'cb_power');

// Configure threshold for power "greater than 10 W"
// with a debounce period of 1s (1000ms)
$vc->setPowerCallbackConfiguration(1000, FALSE, '>', 10*1000, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
