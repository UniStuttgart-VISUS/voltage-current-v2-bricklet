function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletVoltageCurrentV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Voltage/Current Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    vc = handle(BrickletVoltageCurrentV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register current callback to function cb_current
    set(vc, 'CurrentCallback', @(h, e) cb_current(e));

    % Set period for current callback to 1s (1000ms) without a threshold
    vc.setCurrentCallbackConfiguration(1000, false, 'x', 0, 0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for current callback
function cb_current(e)
    fprintf('Current: %g A\n', e.current/1000.0);
end
