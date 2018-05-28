function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Voltage/Current Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    vc = javaObject("com.tinkerforge.BrickletVoltageCurrentV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register current callback to function cb_current
    vc.addCurrentCallback(@cb_current);

    % Set period for current callback to 1s (1000ms) without a threshold
    vc.setCurrentCallbackConfiguration(1000, false, "x", 0, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for current callback
function cb_current(e)
    fprintf("Current: %g A\n", e.current/1000.0);
end
