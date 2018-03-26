/* voltage-current-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/utility/callback_value.h"
#include "bricklib2/logging/logging.h"

#include "ina226.h"

CallbackValue callback_value_current;
CallbackValue callback_value_voltage;
CallbackValue callback_value_power;


BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_CURRENT: return get_callback_value(message, response, &callback_value_current);
		case FID_SET_CURRENT_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration(message, &callback_value_current);
		case FID_GET_CURRENT_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration(message, response, &callback_value_current);
		case FID_GET_VOLTAGE: return get_callback_value(message, response, &callback_value_voltage);
		case FID_SET_VOLTAGE_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration(message, &callback_value_voltage);
		case FID_GET_VOLTAGE_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration(message, response, &callback_value_voltage);
		case FID_GET_POWER: return get_callback_value(message, response, &callback_value_power);
		case FID_SET_POWER_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration(message, &callback_value_power);
		case FID_GET_POWER_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration(message, response, &callback_value_power);
		case FID_SET_CONFIGURATION: return set_configuration(message);
		case FID_GET_CONFIGURATION: return get_configuration(message, response);
		case FID_SET_CALIBRATION: return set_calibration(message);
		case FID_GET_CALIBRATION: return get_calibration(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}

BootloaderHandleMessageResponse set_configuration(const SetConfiguration *data) {
	logd("[+] VC2: set_configuration()\n\r");

	ina226.averaging = data->averaging;
	ina226.voltage_conversion_time = data->voltage_conversion_time;
	ina226.current_conversion_time = data->current_conversion_time;
	ina226.new_configuration = true;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_configuration(const GetConfiguration *data, GetConfiguration_Response *response) {
	logd("[+] VC2: get_configuration()\n\r");

	response->header.length = sizeof(GetConfiguration_Response);
	response->averaging = ina226.averaging;
	response->voltage_conversion_time = ina226.voltage_conversion_time;
	response->current_conversion_time = ina226.current_conversion_time;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_calibration(const SetCalibration *data) {
	logd("[+] VC2: set_calibration()\n\r");

	if(data->voltage_divisor == 0 || data->current_divisor == 0) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	ina226.cal_v_multiplier = data->voltage_multiplier;
	ina226.cal_v_divisor = data->voltage_divisor;
	ina226.cal_c_multiplier = data->current_multiplier;
	ina226.cal_c_divisor = data->current_divisor;

	calibration_eeprom_write();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_calibration(const GetCalibration *data, GetCalibration_Response *response) {
	logd("[+] VC2: get_calibration()\n\r");

	response->header.length = sizeof(GetCalibration_Response);
	response->voltage_multiplier = ina226.cal_v_multiplier;
	response->voltage_divisor = ina226.cal_v_divisor;
	response->current_multiplier = ina226.cal_c_multiplier;
	response->current_divisor = ina226.cal_c_divisor;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

bool handle_current_callback(void) {
	return handle_callback_value_callback(&callback_value_current, FID_CALLBACK_CURRENT);
}

bool handle_voltage_callback(void) {
	return handle_callback_value_callback(&callback_value_voltage, FID_CALLBACK_VOLTAGE);
}

bool handle_power_callback(void) {
	return handle_callback_value_callback(&callback_value_power, FID_CALLBACK_POWER);
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	callback_value_init(&callback_value_current, ina226_get_current);
	callback_value_init(&callback_value_voltage, ina226_get_voltage);
	callback_value_init(&callback_value_power, ina226_get_power);

	communication_callback_init();
}
