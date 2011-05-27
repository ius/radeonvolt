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
#include "i2c.h"
#include "vt1165.h"

const u8 MAX_SLAVE_CURRENT[] = {24, 36, 32, 28, 26, 38, 34, 30};

u8 vt1165_device_id(struct rv8xx_i2c *i2c)
{
	return i2c_read_byte(i2c->base, i2c->address, VT1165_REG_DEVICE_ID);
}

u8 vt1165_radc(struct rv8xx_i2c *i2c, unsigned int index)
{
	unsigned int reg, value = 0;

	switch(index) {
		case 0:
			reg = i2c_read_byte(i2c->base, i2c->address, 0x02);
			value = reg & 0x1f;
			break;
		case 1:
			reg = i2c_read_byte(i2c->base, i2c->address, 0x02);
			value = (reg >> 5);
			reg = i2c_read_byte(i2c->base, i2c->address, 0x03);
			value = ((reg << 3) | value) & 0x1f;
			break;
		case 2:
			reg = i2c_read_byte(i2c->base, i2c->address, 0x03);
			value = (reg >> 2) & 0x1f;
			break;
		case 3:
			reg = i2c_read_byte(i2c->base, i2c->address, 0x03);
			value = (reg >> 7);
			reg = i2c_read_byte(i2c->base, i2c->address, 0x04);
			value = ((reg << 1) | value) & 0x1f;
			break;
		case 4:
			reg = i2c_read_byte(i2c->base, i2c->address, 0x04);
			value = (reg >> 4);
			reg = i2c_read_byte(i2c->base, i2c->address, 0x05);
			value = ((reg << 4) | value) & 0x1f;
			break;
		case 5:
			reg = i2c_read_byte(i2c->base, i2c->address, 0x05);
			value = (reg >> 1) & 0x1f;
			break;
		case 6:
			reg = i2c_read_byte(i2c->base, i2c->address, 0x05);
			value = (reg >> 6);
			reg = i2c_read_byte(i2c->base, i2c->address, 0x06);
			value = ((reg << 2) | value) & 0x1f;
			break;
		case 7:
			reg = i2c_read_byte(i2c->base, i2c->address, 0x06);
			value = (reg >> 3) & 0x1f;
			break;	
	}

	return value;
}

u8 vt1165_vid_mode(struct rv8xx_i2c *i2c)
{
	u8 data = vt1165_radc(i2c, 3);
	return (data >> 2);
}

u8 vt1165_active_slaves(struct rv8xx_i2c *i2c)
{
	return i2c_read_byte(i2c->base, i2c->address, 0x0e);
}

u8 vt1165_temp(struct rv8xx_i2c *i2c, u8 slave)
{
	i2c_write_byte(i2c->base, i2c->address, 0x08, 0x00);
	i2c_write_byte(i2c->base, i2c->address, 0x07, 0xA7);
	i2c_write_byte(i2c->base, i2c->address, 0x08, (0x80 | (slave + 1)));

	return i2c_read_byte(i2c->base, i2c->address, 0x0a);
}

unsigned int vt1165_max_current(struct rv8xx_i2c *i2c)
{
	u8 data;

	data = vt1165_radc(i2c, 4);

	return MAX_SLAVE_CURRENT[(data >> 2) & 0x07] * vt1165_active_slaves(i2c);
}

float vt1165_avg_current(struct rv8xx_i2c *i2c)
{
	u8 data;

	data = i2c_read_byte(i2c->base, i2c->address, 0x1e);

	return ((data & 0x7f) / 124.0) * vt1165_max_current(i2c);
}

u8 vt1165_current_preset(struct rv8xx_i2c *i2c)
{
	u8 data;

	data = i2c_read_byte(i2c->base, i2c->address, 0x1c);
	data = ((data >> 4) & 0x03);

	return data;
}

float vt1165_get_voltage(struct rv8xx_i2c *i2c, u8 index)
{
	u8 data;

	data = i2c_read_byte(i2c->base, i2c->address, 0x15 + index);

	return (0.450f + 0.0125f * (data & 0x7f));
}

void vt1165_set_voltage(struct rv8xx_i2c *i2c, u8 index, u8 value)
{
	i2c_write_byte(i2c->base, i2c->address, 0x15 + (index & 0x03), value);
}
