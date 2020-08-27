#include "bindings/hal_common.h"
#include "bindings/bricklet_voltage_current_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Voltage/Current Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


// Callback function for power callback
static void power_handler(TF_VoltageCurrentV2 *device, int32_t power, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("power: %d 1/%d W\n", power, 1000.0);
}

static TF_VoltageCurrentV2 vc;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_voltage_current_v2_create(&vc, UID, hal), "create device object");

	// Register power callback to function power_handler
	tf_voltage_current_v2_register_power_callback(&vc,
	                                              power_handler,
	                                              NULL);

	// Configure threshold for power "greater than 10 W"
	// with a debounce period of 1s (1000ms)
	tf_voltage_current_v2_set_power_callback_configuration(&vc, 1000, false, '>', 10*1000, 0);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
