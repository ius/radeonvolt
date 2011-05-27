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
#include "types.h"

#define VT1165_ADDRESS			0x70
#define VT1165_REG_DEVICE_ID		0x1a

#define VT1165_DEVICE_ID		0x0a

/**
 * Returns the value of the device id register
 * @param i2c i2c context
 * @return device id
 */
u8 vt1165_device_id(struct rv8xx_i2c *i2c);

/**
 * Returns the current VID mode
 * @param i2c i2c context
 * @return VID mode
 */
u8 vt1165_vid_mode(struct rv8xx_i2c *i2c);

/**
 * Returns the temperature of specified slave
 * @param i2c i2c context
 * @param slave slave index
 * @return temperature in degrees centigrade
 */
u8 vt1165_temp(struct rv8xx_i2c *i2c, u8 slave);

/**
 * Returns the number of active slaves
 * @param i2c i2c context
 * @return number of active slaves
 */
u8 vt1165_active_slaves(struct rv8xx_i2c *i2c);

/**
 * Returns the maximum output current of all slaves combined
 * @param i2c i2c context
 * @return maximum output current
 */
unsigned int vt1165_max_current(struct rv8xx_i2c *i2c);

/**
 * Returns the current average over an unspecified timeframe
 * @param i2c i2c context
 * @return current average
 */
float vt1165_avg_current(struct rv8xx_i2c *i2c);

/**
 * Returns the current voltage preset index
 * @param i2c i2c context
 */
u8 vt1165_current_preset(struct rv8xx_i2c *i2c);

/**
 * Returns the specified voltage preset
 * @param i2c i2c context
 * @param index preset index
 * @return regulator output voltage
 */
float vt1165_get_voltage(struct rv8xx_i2c *i2c, u8 index);

/**
 * Sets the voltage modifier value for the specified preset
 * @param i2c i2c context
 * @param index preset index
 * @param value Voltage index
 */
void vt1165_set_voltage(struct rv8xx_i2c *i2c, u8 index, u8 value);

