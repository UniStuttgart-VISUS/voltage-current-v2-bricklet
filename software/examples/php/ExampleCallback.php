<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletVoltageCurrentV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletVoltageCurrentV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Voltage/Current Bricklet 2.0

// Callback function for current callback
function cb_current($current)
{
    echo "Current: " . $current/1000.0 . " A\n";
}

$ipcon = new IPConnection(); // Create IP connection
$vc = new BrickletVoltageCurrentV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register current callback to function cb_current
$vc->registerCallback(BrickletVoltageCurrentV2::CALLBACK_CURRENT, 'cb_current');

// Set period for current callback to 1s (1000ms) without a threshold
$vc->setCurrentCallbackConfiguration(1000, FALSE, 'x', 0, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
