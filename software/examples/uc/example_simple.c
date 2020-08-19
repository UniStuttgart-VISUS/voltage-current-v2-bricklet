

#include "bindings/hal_common.h"
#include "bindings/bricklet_voltage_current_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Voltage/Current Bricklet 2.0

void check(int rc, const char* msg);




TF_VoltageCurrentV2 vc;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_voltage_current_v2_create(&vc, UID, hal), "create device object");


	// Get current voltage
	int32_t voltage;
	check(tf_voltage_current_v2_get_voltage(&vc, &voltage), "get voltage");

	tf_hal_printf("Voltage: %d 1/%d V\n", voltage, 1000.0);

	// Get current current
	int32_t current;
	check(tf_voltage_current_v2_get_current(&vc, &current), "get current");

	tf_hal_printf("Current: %d 1/%d A\n", current, 1000.0);

}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
