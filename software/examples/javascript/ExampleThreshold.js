var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Voltage/Current Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var vc = new Tinkerforge.BrickletVoltageCurrentV2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Configure threshold for power "greater than 10 W"
        // with a debounce period of 1s (1000ms)
        vc.setPowerCallbackConfiguration(1000, false, '>', 10*1000, 0);
    }
);

// Register power callback
vc.on(Tinkerforge.BrickletVoltageCurrentV2.CALLBACK_POWER,
    // Callback function for power callback
    function (power) {
        console.log('power: ' + power/1000.0 + ' W');
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
