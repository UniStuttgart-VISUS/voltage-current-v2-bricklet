<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletVoltageCurrentV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletVoltageCurrentV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Voltage/Current Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$vc = new BrickletVoltageCurrentV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current voltage
$voltage = $vc->getVoltage();
echo "Voltage: " . $voltage/1000.0 . " V\n";

// Get current current
$current = $vc->getCurrent();
echo "Current: " . $current/1000.0 . " A\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
