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
#include <stdio.h>
#include <unistd.h>

#include "i2c.h"

void i2c_print_status(u32 status)
{
	printf("i2c status: ");

	switch(status & I2C_STATUS) {
		case 0:	printf("idle");
			break;
		case 1:
			printf("sending start");
			break;
		case 2:
			printf("sending address");
			break;
		case 3:
			printf("sending/receiving data");
			break;
		case 4:
			printf("sending/receiving ack");
			break;
		case 5:
			printf("sending stop");
			break;
		case 7:
			printf("byte delay");
			break;
		case 8:
			printf("waiting for GO");
			break;
		default:
			printf("unknown");
			break;
	}

	if(status & I2C_DONE) {
		printf(" (done)");
	}

	if(status & I2C_ABORTED) {
		printf(" (aborted)");
	}

	if(status & I2C_TIMEOUT) {
		printf(" (timeout)");
	}

	if(status & I2C_STOPPED_ON_NACK) {
		printf(" (stopped on nack)");
	}

	if(status & I2C_NACK) {
		printf(" (nack)");
	}

	printf("\n");
}

u32 mmio_read(void *base, u32 offset)
{
	u32 value;

	value = *(u32*)(base+offset);
	//printf("R %04X %X\n", offset, value);
	return value;	
}

void mmio_write(void *base, u32 offset, u32 value)
{
	*(u32*)(base+offset) = value;
	//printf("W %04X %X\n", offset, value);
	usleep(1);
}

void i2c_show_status(void *base)
{
	i2c_print_status(mmio_read(base, RV8XX_I2C_STATUS));
}

void i2c_do_tx(void *base)
{
	mmio_write(base, RV8XX_I2C_CONTROL, I2C_GO);

	usleep(1000);
	//i2c_show_status(base);
}

void i2c_setup(void *base)
{
	u32 reg;

	reg = mmio_read(base, RV8XX_I2C_INTERRUPT_CONTROL);
	mmio_write(base, RV8XX_I2C_INTERRUPT_CONTROL, reg | I2C_DONE_MASK);

	/* Set prescale, threshold (1/2 of samples) */
	mmio_write(base, RV8XX_I2C_SPEED, ((0x021c << 16) | 0x02));

	reg = mmio_read(base, RV8XX_I2C_SETUP);
	mmio_write(base, RV8XX_I2C_SETUP, 0x28010007);
}

void i2c_select_bus(void *base, unsigned int bus)
{
	u32 reg;

	/* Note: bus/pins are currently hardcoded */
	reg = mmio_read(base, RV8XX_DC_GPIO_DDC1_MASK);
	mmio_write(base, RV8XX_DC_GPIO_DDC1_MASK, reg & 0xff000000); /* Not in documentation? */

	mmio_write(base, RV8XX_I2C_PIN_SELECTION, (RV8XX_GPIO_PIN_SDA << 8) | RV8XX_GPIO_PIN_SCL);
}

void i2c_write(void *base, u8 address, u8 *data, u8 length)
{
	int i;

	if(length > 12)
		return;

	mmio_write(base, RV8XX_I2C_TRANSACTION, (length << 16) | I2C_START | I2C_STOP);
	mmio_write(base, RV8XX_I2C_DATA, I2C_INDEX_WRITE | ((address << 1) << 8));

	for(i = 0; i < length; i++) {
		mmio_write(base, RV8XX_I2C_DATA, (data[i] << 8));
	}

	i2c_do_tx(base);
}

void i2c_write_byte(void *base, u8 address, u8 offset, u8 value)
{
	u8 buf[2];

	buf[0] = offset;
	buf[1] = value;

	i2c_write(base, address, buf, sizeof(buf));
}

u8 i2c_read_byte(void *base, u8 address, u8 offset)
{
	u32 reg;

	/* Write offset */
	mmio_write(base, RV8XX_I2C_TRANSACTION, (1 << 16) | I2C_START);
	mmio_write(base, RV8XX_I2C_DATA, I2C_INDEX_WRITE | ((address << 1) << 8));
	mmio_write(base, RV8XX_I2C_DATA, (offset << 8));

	i2c_do_tx(base);

	/* Read back */
	mmio_write(base, RV8XX_I2C_TRANSACTION, (1 << 16) | I2C_START | I2C_STOP | I2C_RW);
	mmio_write(base, RV8XX_I2C_DATA, I2C_INDEX_WRITE | (((address << 1) | 0x01 ) << 8));
	i2c_do_tx(base);
	
	mmio_write(base, RV8XX_I2C_DATA, I2C_INDEX_WRITE | (0x01 << 16) | I2C_DATA_RW);

	reg = mmio_read(base, RV8XX_I2C_DATA);
	return (reg >> 8);
}

