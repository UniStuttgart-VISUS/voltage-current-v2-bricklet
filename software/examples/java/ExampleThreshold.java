import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletVoltageCurrentV2;

public class ExampleThreshold {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Voltage/Current Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletVoltageCurrentV2 vc =
		  new BrickletVoltageCurrentV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Add power listener
		vc.addPowerListener(new BrickletVoltageCurrentV2.PowerListener() {
			public void power(int power) {
				System.out.println("power: " + power/1000.0 + " W");
			}
		});

		// Configure threshold for power "greater than 10 W"
		// with a debounce period of 1s (1000ms)
		vc.setPowerCallbackConfiguration(1000, false, '>', 10*1000, 0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
