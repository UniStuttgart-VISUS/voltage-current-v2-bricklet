/* voltage-current-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define VOLTAGE_CURRENT_V2_THRESHOLD_OPTION_OFF 'x'
#define VOLTAGE_CURRENT_V2_THRESHOLD_OPTION_OUTSIDE 'o'
#define VOLTAGE_CURRENT_V2_THRESHOLD_OPTION_INSIDE 'i'
#define VOLTAGE_CURRENT_V2_THRESHOLD_OPTION_SMALLER '<'
#define VOLTAGE_CURRENT_V2_THRESHOLD_OPTION_GREATER '>'

#define VOLTAGE_CURRENT_V2_AVERAGING_1 0
#define VOLTAGE_CURRENT_V2_AVERAGING_4 1
#define VOLTAGE_CURRENT_V2_AVERAGING_16 2
#define VOLTAGE_CURRENT_V2_AVERAGING_64 3
#define VOLTAGE_CURRENT_V2_AVERAGING_128 4
#define VOLTAGE_CURRENT_V2_AVERAGING_256 5
#define VOLTAGE_CURRENT_V2_AVERAGING_512 6
#define VOLTAGE_CURRENT_V2_AVERAGING_1024 7

#define VOLTAGE_CURRENT_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define VOLTAGE_CURRENT_V2_BOOTLOADER_MODE_FIRMWARE 1
#define VOLTAGE_CURRENT_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define VOLTAGE_CURRENT_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define VOLTAGE_CURRENT_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define VOLTAGE_CURRENT_V2_BOOTLOADER_STATUS_OK 0
#define VOLTAGE_CURRENT_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define VOLTAGE_CURRENT_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define VOLTAGE_CURRENT_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define VOLTAGE_CURRENT_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define VOLTAGE_CURRENT_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define VOLTAGE_CURRENT_V2_STATUS_LED_CONFIG_OFF 0
#define VOLTAGE_CURRENT_V2_STATUS_LED_CONFIG_ON 1
#define VOLTAGE_CURRENT_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define VOLTAGE_CURRENT_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_CURRENT 1
#define FID_SET_CURRENT_CALLBACK_CONFIGURATION 2
#define FID_GET_CURRENT_CALLBACK_CONFIGURATION 3
#define FID_GET_VOLTAGE 5
#define FID_SET_VOLTAGE_CALLBACK_CONFIGURATION 6
#define FID_GET_VOLTAGE_CALLBACK_CONFIGURATION 7
#define FID_GET_POWER 9
#define FID_SET_POWER_CALLBACK_CONFIGURATION 10
#define FID_GET_POWER_CALLBACK_CONFIGURATION 11
#define FID_SET_CONFIGURATION 13
#define FID_GET_CONFIGURATION 14
#define FID_SET_CALIBRATION 15
#define FID_GET_CALIBRATION 16

#define FID_CALLBACK_CURRENT 4
#define FID_CALLBACK_VOLTAGE 8
#define FID_CALLBACK_POWER 12

typedef struct {
	TFPMessageHeader header;
	uint8_t averaging;
	uint8_t voltage_conversion_time;
	uint8_t current_conversion_time;
} __attribute__((__packed__)) SetConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetConfiguration;

typedef struct {
	TFPMessageHeader header;
	uint8_t averaging;
	uint8_t voltage_conversion_time;
	uint8_t current_conversion_time;
} __attribute__((__packed__)) GetConfiguration_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t gain_multiplier;
	uint16_t gain_divisor;
} __attribute__((__packed__)) SetCalibration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetCalibration;

typedef struct {
	TFPMessageHeader header;
	uint16_t gain_multiplier;
	uint16_t gain_divisor;
} __attribute__((__packed__)) GetCalibration_Response;


// Function prototypes
BootloaderHandleMessageResponse set_configuration(const SetConfiguration *data);
BootloaderHandleMessageResponse get_configuration(const GetConfiguration *data, GetConfiguration_Response *response);
BootloaderHandleMessageResponse set_calibration(const SetCalibration *data);
BootloaderHandleMessageResponse get_calibration(const GetCalibration *data, GetCalibration_Response *response);

// Callbacks
bool handle_current_callback(void);
bool handle_voltage_callback(void);
bool handle_power_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 3
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_current_callback, \
	handle_voltage_callback, \
	handle_power_callback, \


#endif
