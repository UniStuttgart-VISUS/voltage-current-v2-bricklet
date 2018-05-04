/* voltage-current-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 * Copyright (C) 2018 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * ina226.c: INA226 driver
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

#include "ina226.h"

#include "configs/config_ina226.h"
#include "bricklib2/logging/logging.h"
#include "bricklib2/bootloader/bootloader.h"

#include <string.h>

INA226 ina226;

void calibration_eeprom_write(void) {
	logd("[+] VC2: calibration_eeprom_write()\n\r");

	uint32_t page[EEPROM_PAGE_SIZE/sizeof(uint32_t)];

	page[CALIBRATION_MAGIC_POS] = CALIBRATION_MAGIC;
	page[CALIBRATION_V_MULTIPLIER_POS] = ina226.cal_v_multiplier;
	page[CALIBRATION_V_DIVISOR_POS] = ina226.cal_v_divisor;
	page[CALIBRATION_C_MULTIPLIER_POS] = ina226.cal_c_multiplier;
	page[CALIBRATION_C_DIVISOR_POS] = ina226.cal_c_divisor;

	if(!bootloader_write_eeprom_page(CALIBRATION_PAGE, page)) {
		// TODO: Error handling?
		return;
	}
}

void calibration_eeprom_read(void) {
	logd("[+] VC2: calibration_read()\n\r");

	uint32_t page[EEPROM_PAGE_SIZE/sizeof(uint32_t)];

	bootloader_read_eeprom_page(CALIBRATION_PAGE, page);

	// The magic number is not where it is supposed to be.
	// This is either our first startup or something went wrong.
	// We initialize the calibration data with sane default values.
	if(page[0] != CALIBRATION_MAGIC) {
		ina226.cal_v_multiplier = 1;
		ina226.cal_v_divisor = 1;
		ina226.cal_c_multiplier = 1;
		ina226.cal_c_divisor = 1;

		calibration_eeprom_write();

		return;
	}

	ina226.cal_v_multiplier = page[CALIBRATION_V_MULTIPLIER_POS];
	ina226.cal_v_divisor = page[CALIBRATION_V_DIVISOR_POS];
	ina226.cal_c_multiplier = page[CALIBRATION_C_MULTIPLIER_POS];
	ina226.cal_c_divisor = page[CALIBRATION_C_DIVISOR_POS];
}

static void ina226_write_register_be(const uint8_t reg, const uint16_t value) {
	uint8_t value_be[2];
	value_be[0] = ((uint8_t*)&value)[1];
	value_be[1]= ((uint8_t*)&value)[0];

	i2c_fifo_write_register(&ina226.i2c_fifo, reg, 2, value_be, true);
}

void ina226_init(void) {
	logd("[+] VC2: ina226_init()\n\r");

	memset(&ina226, 0, sizeof(INA226));

	ina226.averaging                 = INA226_DEFAULT_AVERAGING;
	ina226.voltage_conversion_time   = INA226_DEFAULT_CONVERSION_BV;
	ina226.current_conversion_time   = INA226_DEFAULT_CONVERSION_SV;

	ina226.i2c_fifo.baudrate         = INA226_I2C_BAUDRATE;
	ina226.i2c_fifo.address          = INA226_I2C_ADDRESS;
	ina226.i2c_fifo.i2c              = INA226_I2C;

	ina226.i2c_fifo.scl_port         = INA226_SCL_PORT;
	ina226.i2c_fifo.scl_pin          = INA226_SCL_PIN;
	ina226.i2c_fifo.scl_mode         = INA226_SCL_PIN_MODE;
	ina226.i2c_fifo.scl_input        = INA226_SCL_INPUT;
	ina226.i2c_fifo.scl_source       = INA226_SCL_SOURCE;
	ina226.i2c_fifo.scl_fifo_size    = INA226_SCL_FIFO_SIZE;
	ina226.i2c_fifo.scl_fifo_pointer = INA226_SCL_FIFO_POINTER;

	ina226.i2c_fifo.sda_port         = INA226_SDA_PORT;
	ina226.i2c_fifo.sda_pin          = INA226_SDA_PIN;
	ina226.i2c_fifo.sda_mode         = INA226_SDA_PIN_MODE;
	ina226.i2c_fifo.sda_input        = INA226_SDA_INPUT;
	ina226.i2c_fifo.sda_source       = INA226_SDA_SOURCE;
	ina226.i2c_fifo.sda_fifo_size    = INA226_SDA_FIFO_SIZE;
	ina226.i2c_fifo.sda_fifo_pointer = INA226_SDA_FIFO_POINTER;

	ina226.cal_v_multiplier = 1;
	ina226.cal_v_divisor = 1;
	ina226.cal_c_multiplier = 1;
	ina226.cal_c_divisor = 1;

	calibration_eeprom_read();

	i2c_fifo_init(&ina226.i2c_fifo);

	const XMC_GPIO_CONFIG_t alert_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD,
	};

	XMC_GPIO_Init(INA226_ALERT_PIN, &alert_pin_config);

	ina226.state             = INA226_STATE_WAIT_FOR_ALERT;
	ina226.new_calibration   = true;
	ina226.new_configuration = true;
	ina226.new_mask          = true;
}

void ina226_tick(void) {
	I2CFifoState state = i2c_fifo_next_state(&ina226.i2c_fifo);

	// TODO: Add timeout as error condition?
	if(state & I2C_FIFO_STATE_ERROR) {
		loge("INA226 I2C error: %d\n\r", state);
		ina226_init();
		return;
	}

	if(ina226.state != INA226_STATE_WAIT_FOR_ALERT) {
		switch(state) {
			case I2C_FIFO_STATE_READ_REGISTER_READY: {
				uint8_t buffer[16];
				uint8_t length = i2c_fifo_read_fifo(&ina226.i2c_fifo, buffer, 16);
				if(length != 2) {
					loge("INA226 I2C unexpected read length : %d (state: %d)\n\r", length, ina226.state);
					ina226_init();
					break;
				}

				switch(ina226.state) {
					case INA226_STATE_READ_VOLTAGE: {
						ina226.voltage = ((((buffer[0] << 8) | buffer[1]) * VOLTAGE_ADC_MV_MUL) / VOLTAGE_ADC_MV_DIV) * ina226.cal_v_multiplier / ina226.cal_v_divisor;
						ina226.state = INA226_STATE_READ_CURRENT;
						//logd("voltage: %d\n\r", ina226.voltage);
						break;
					}

					case INA226_STATE_READ_CURRENT: {
						ina226.current = ((((buffer[0] << 8) | buffer[1]) * CURRENT_ADC_MA_MUL) / CURRENT_ADC_MA_DIV) * ina226.cal_c_multiplier / ina226.cal_c_divisor;
						ina226.power = (ina226.voltage * ina226.current) / 1000;
						ina226.state = INA226_STATE_READ_MASK;

						if (ina226.current > 40000) {
							ina226.current = 0;
						}

						//logd("current: %d\n\r", ina226.current);

						break;
					}

					case INA226_STATE_READ_MASK: {
						ina226.state = INA226_STATE_WAIT_FOR_ALERT;
						//logd("mask: %d\n\r", (buffer[0] << 8) | buffer[1]);
						break;
					}

					case INA226_STATE_WAIT_FOR_ALERT: {
						// This should be unreachable. Do we want to reinit i2c here?
						break;
					}
				}

				break;
			}

			case I2C_FIFO_STATE_WRITE_REGISTER_READY: {
				// Nothing to do here.
				break;
			}

			case I2C_FIFO_STATE_IDLE: {
				break; // Handled below
			}

			default: {
				// If we end up in a ready state that we don't handle, something went wrong
				if(state & I2C_FIFO_STATE_READY) {
					loge("INA226 I2C unrecognized ready state : %d\n\r", state);
					ina226_init();
				}
				return;
			}
		}
	}

	if((state == I2C_FIFO_STATE_IDLE) || (state & I2C_FIFO_STATE_READY)) {
		if(ina226.new_calibration) {
			// 2 Milliohm
			// (((0.002/(2.5/1000/1000))*2097.152)/2048)*40.0 = 2^15

			// 4 Milliohm
			// (((0.004/(2.5/1000/1000))*2097.152)/2048)*20.0 = 2^15
			ina226_write_register_be(INA226_REG_CALIBRATION, 2048);
			ina226.new_calibration = false;
		} else if(ina226.new_configuration) {
			ina226_write_register_be(INA226_REG_CONFIGURATION,
			                         (INA226_CONF_AVERAGING(ina226.averaging) |
			                          INA226_CONF_CONVERSION_BV(ina226.voltage_conversion_time) |
			                          INA226_CONF_CONVERSION_SV(ina226.current_conversion_time) |
			                          INA226_CONF_OPERATING_MODE(INA226_DEFAULT_OPERATING_MODE)));
			ina226.new_configuration = false;
		} else if(ina226.new_mask) {
			ina226_write_register_be(INA226_REG_MASK_ENABLE, INA226_MASK_CONVERSION_READY);
			ina226.new_mask = false;
		} else {
			switch(ina226.state) {
				case INA226_STATE_WAIT_FOR_ALERT: {
					if(!XMC_GPIO_GetInput(INA226_ALERT_PIN)) {
						ina226.state = INA226_STATE_READ_VOLTAGE;
					} else {
						break;
					}
				} // fall-through if alert pin low
				case INA226_STATE_READ_VOLTAGE: {
					i2c_fifo_read_register(&ina226.i2c_fifo, INA226_REG_BUS_VOLTAGE, 2);
					break;
				}

				case INA226_STATE_READ_CURRENT: {
					i2c_fifo_read_register(&ina226.i2c_fifo, INA226_REG_CURRENT, 2);
					break;
				}

				case INA226_STATE_READ_MASK: {
					i2c_fifo_read_register(&ina226.i2c_fifo, INA226_REG_MASK_ENABLE, 2);
					break;
				}
			}
		}
	}
}

int32_t ina226_get_voltage(void) {
	return ina226.voltage;
}

int32_t ina226_get_current(void) {
	return ina226.current;
}

int32_t ina226_get_power(void) {
	return ina226.power;
}
