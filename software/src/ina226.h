/* voltage-current-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * ina226.h: INA226 driver
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

#ifndef INA226_H
#define INA226_H

#include "bricklib2/hal/i2c_fifo/i2c_fifo.h"

#include <stdint.h>

#define CALIBRATION_PAGE 1
#define CALIBRATION_MAGIC 0x12345678
#define CALIBRATION_MAGIC_POS 0
#define CALIBRATION_V_MULTIPLIER_POS 1
#define CALIBRATION_V_DIVISOR_POS 2
#define CALIBRATION_C_MULTIPLIER_POS 3
#define CALIBRATION_C_DIVISOR_POS 4

/*
 * The following multipliers and divisors are used
 * to convert the raw digital current and voltage
 * readings to to mA and mV readings respectively.
 */
#define CURRENT_ADC_MA_MUL 5
#define CURRENT_ADC_MA_DIV 8
#define VOLTAGE_ADC_MV_MUL 5
#define VOLTAGE_ADC_MV_DIV 4

#define INA226_REG_CONFIGURATION 0x00
#define INA226_REG_SHUNT_VOLTAGE 0x01
#define INA226_REG_BUS_VOLTAGE   0x02
#define INA226_REG_POWER         0x03
#define INA226_REG_CURRENT       0x04
#define INA226_REG_CALIBRATION   0x05
#define INA226_REG_MASK_ENABLE   0x06
#define INA226_REG_ALERT_LIMIT   0x07
#define INA226_REG_DIE_ID        0xFF

#define INA226_CONF_AVERAGING(val)         ((val) << 9)
#define INA226_CONF_CONVERSION_BV(val)     ((val) << 6)
#define INA226_CONF_CONVERSION_SV(val)     ((val) << 3)
#define INA226_CONF_OPERATING_MODE(val)    ((val) << 0)

#define INA226_DEFAULT_AVERAGING           3
#define INA226_DEFAULT_CONVERSION_BV       4
#define INA226_DEFAULT_CONVERSION_SV       4
#define INA226_DEFAULT_OPERATING_MODE      7

#define INA226_MASK_SV_OVER_VOLTAGE        1 << 15
#define INA226_MASK_SV_UNDER_VOLTAGE       1 << 14
#define INA226_MASK_BV_OVER_VOLTAGE        1 << 13
#define INA226_MASK_BV_UNDER_VOLTAGE       1 << 12
#define INA226_MASK_OVER_LIMIT_POWER       1 << 11
#define INA226_MASK_CONVERSION_READY       1 << 10
#define INA226_MASK_ALERT_FUNCTION_FLAG    1 << 4
#define INA226_MASK_CONVERSION_READY_FLAG  1 << 3
#define INA226_MASK_MATH_OVERFLOW_FLAG     1 << 2
#define INA226_MASK_ALERT_POLARITY         1 << 1
#define INA226_MASK_ALTER_LATCH_ENABLE     1 << 0

typedef enum {
	INA226_STATE_READ_VOLTAGE,
	INA226_STATE_READ_CURRENT,
	INA226_STATE_READ_MASK,
	INA226_STATE_WAIT_FOR_ALERT,
} INA226State;

typedef struct {
	int32_t voltage;
	int32_t current;
	int32_t power;
	uint32_t power_time;
	bool new_calibration;
	bool new_configuration;
	bool new_mask;

	uint16_t cal_v_multiplier;
	uint16_t cal_v_divisor;
	uint16_t cal_c_multiplier;
	uint16_t cal_c_divisor;

	uint8_t averaging;
	uint8_t voltage_conversion_time;
	uint8_t current_conversion_time;

	I2CFifo i2c_fifo;
	INA226State state;
} INA226;

extern INA226 ina226;

void ina226_init(void);
void ina226_tick(void);

int32_t ina226_get_voltage(void);
int32_t ina226_get_current(void);
int32_t ina226_get_power(void);

void calibration_eeprom_read(void);
void calibration_eeprom_write(void);

#endif
