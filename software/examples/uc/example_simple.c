// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_voltage_current_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_VoltageCurrentV2 vc;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_voltage_current_v2_create(&vc, NULL, hal), "create device object");

	// Get current voltage
	int32_t voltage;
	check(tf_voltage_current_v2_get_voltage(&vc, &voltage), "get voltage");

	tf_hal_printf("Voltage: %d 1/%d V\n", voltage, 1000);

	// Get current current
	int32_t current;
	check(tf_voltage_current_v2_get_current(&vc, &current), "get current");

	tf_hal_printf("Current: %d 1/%d A\n", current, 1000);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
