/**
 * Radeonvolt - (c) 2011 Joerie de Gram <j.de.gram@gmail.com>
 *
 * Radeonvolt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Radeonvolt is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Radeonvolt.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef I2C_H
#define I2C_H

#include "types.h"

#define RV8XX_GPIO_PIN_SCL	0x29
#define RV8XX_GPIO_PIN_SDA	0x28

#define RV8XX_I2C_INTERRUPT_CONTROL	0x60d4
#define RV8XX_I2C_CONTROL		0x60d0
#define RV8XX_I2C_STATUS		0x60d8
#define RV8XX_I2C_SPEED			0x60dc
#define RV8XX_I2C_SETUP			0x60e0
#define RV8XX_I2C_TRANSACTION		0x60e4
#define RV8XX_I2C_DATA			0x60e8
#define RV8XX_I2C_PIN_SELECTION		0x60ec
#define RV8XX_DC_GPIO_DDC1_MASK		0x6430

#define I2C_DONE_INT			0x01
#define I2C_DONE_ACK			(0x01 << 1)
#define I2C_DONE_MASK			(0x01 << 2)

#define I2C_THRESHOLD			0x03
#define I2C_PRESCALE			(0xffff << 16)

/* i2c control */
#define I2C_GO				1
#define I2C_SOFT_RESET			(1 << 1)
#define I2C_SEND_RESET			(1 << 2)

/* i2c status */
#define I2C_STATUS			0x0f
#define I2C_DONE			(1 << 4)
#define I2C_ABORTED			(1 << 5)
#define I2C_TIMEOUT			(1 << 6)
#define I2C_STOPPED_ON_NACK		(1 << 7)
#define I2C_NACK			(1 << 8)

/* i2c pin selection */
#define I2C_SCL_PIN_SEL			0x7f
#define I2C_SDA_PIN_SEL			(0x7f << 8)

/* i2c transaction */
#define I2C_RW				1
#define I2C_START			(1 << 12)
#define I2C_STOP			(1 << 13)

/* i2c data */
#define I2C_DATA_RW			1
#define I2C_INDEX_WRITE			(1 << 31)

struct rv8xx_i2c {
	void *base;
	u8 address;
};

/**
 * Configures the i2c controller
 * @param base Radeon I/O memory base
 */
void i2c_setup(void *base);

/**
 * Selects an i2c bus
 * NOTE: Currently the bus number is ignored
 * @param base Radeon I/O memory base
 * @param bus i2c bus number
 */
void i2c_select_bus(void *base, unsigned int bus);

/**
 * Writes bytes to specified i2c slave
 * @param base Radeon I/O memory base
 * @param address 7-bit i2c slave address
 * @param data data to write
 * @param length length of data
 */
void i2c_write(void *base, u8 address, u8 *data, u8 length);

/**
 * Writes a single byte to specified i2c slave/register
 * @param base Radeon I/O memory base
 * @param address 7-bit i2c slave address
 * @param offset register address
 * @param value value to write
 */
void i2c_write_byte(void *base, u8 address, u8 offset, u8 value);

/**
 * Reads a single byte from specified i2c slave
 * @param base Radeon I/O memory base
 * @param address 7-bit i2c slave address
 * @param offset register address
 * @return byte read
 */
u8 i2c_read_byte(void *base, u8 address, u8 offset);

#endif

