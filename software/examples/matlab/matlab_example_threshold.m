function matlab_example_threshold()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletVoltageCurrentV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Voltage/Current Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    vc = handle(BrickletVoltageCurrentV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register power callback to function cb_power
    set(vc, 'PowerCallback', @(h, e) cb_power(e));

    % Configure threshold for power "greater than 10 W"
    % with a debounce period of 1s (1000ms)
    vc.setPowerCallbackConfiguration(1000, false, '>', 10*1000, 0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for power callback
function cb_power(e)
    fprintf('power: %g W\n', e.power/1000.0);
end
