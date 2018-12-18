package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"github.com/Tinkerforge/go-api-bindings/voltage_current_v2_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Voltage/Current Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	vc, _ := voltage_current_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	vc.RegisterPowerCallback(func(power int32) {
		fmt.Printf("power: %f W\n", float64(power)/1000.0)
	})

	// Configure threshold for power "greater than 10 W"
	// with a debounce period of 1s (1000ms).
	vc.SetPowerCallbackConfiguration(1000, false, '>', 10*1000, 0)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()

}
