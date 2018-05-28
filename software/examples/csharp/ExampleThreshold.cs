using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Voltage/Current Bricklet 2.0

	// Callback function for power callback
	static void PowerCB(BrickletVoltageCurrentV2 sender, int power)
	{
		Console.WriteLine("power: " + power/1000.0 + " W");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletVoltageCurrentV2 vc =
		  new BrickletVoltageCurrentV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register power callback to function PowerCB
		vc.PowerCallback += PowerCB;

		// Configure threshold for power "greater than 10 W"
		// with a debounce period of 1s (1000ms)
		vc.SetPowerCallbackConfiguration(1000, false, '>', 10*1000, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
