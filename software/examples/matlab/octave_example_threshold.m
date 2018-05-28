function octave_example_threshold()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Voltage/Current Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    vc = javaObject("com.tinkerforge.BrickletVoltageCurrentV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register power callback to function cb_power
    vc.addPowerCallback(@cb_power);

    % Configure threshold for power "greater than 10 W"
    % with a debounce period of 1s (1000ms)
    vc.setPowerCallbackConfiguration(1000, false, ">", 10*1000, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for power callback
function cb_power(e)
    fprintf("power: %g W\n", e.power/1000.0);
end
