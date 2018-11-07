use std::{error::Error, io, thread};
use tinkerforge::{ipconnection::IpConnection, voltage_current_v2_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Voltage/Current Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let voltage_current_v2_bricklet = VoltageCurrentV2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for power events.
    let power_listener = voltage_current_v2_bricklet.get_power_receiver();
    // Spawn thread to handle received events. This thread ends when the voltage_current_v2_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in power_listener {
            println!("power: {}{}", event as f32 / 1000.0, " W");
        }
    });

    // Configure threshold for power "greater than 10 W"
    // with a debounce period of 1s (1000ms)
    voltage_current_v2_bricklet.set_power_callback_configuration(1000, false, '>', 10 * 1000, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
