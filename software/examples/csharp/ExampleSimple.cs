using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Voltage/Current Bricklet 2.0

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletVoltageCurrentV2 vc =
		  new BrickletVoltageCurrentV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get current voltage
		int voltage = vc.GetVoltage();
		Console.WriteLine("Voltage: " + voltage/1000.0 + " V");

		// Get current current
		int current = vc.GetCurrent();
		Console.WriteLine("Current: " + current/1000.0 + " A");

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
