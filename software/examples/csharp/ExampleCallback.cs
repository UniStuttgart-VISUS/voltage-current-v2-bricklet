using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Voltage/Current Bricklet 2.0

	// Callback function for current callback
	static void CurrentCB(BrickletVoltageCurrentV2 sender, int current)
	{
		Console.WriteLine("Current: " + current/1000.0 + " A");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletVoltageCurrentV2 vc =
		  new BrickletVoltageCurrentV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register current callback to function CurrentCB
		vc.CurrentCallback += CurrentCB;

		// Set period for current callback to 1s (1000ms) without a threshold
		vc.SetCurrentCallbackConfiguration(1000, false, 'x', 0, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
