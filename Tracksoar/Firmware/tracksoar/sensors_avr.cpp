/* trackuino copyright (C) 2010  EA5HAV Javi
 * tracksoar sensor changes copyright (C) 2015 Nick Winters
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifdef AVR

#include "sensors_avr.h"
#include <Arduino.h>

#ifdef TRACKSOAR_12
#define BME280_I2C_ADDRESS  0x76

 	Adafruit_BME280 bme280;

	void sensors_setup() {
    // Note: Adafruit_BME280::begin() will call Adafruit_BME280::setSampling() with reasonable defaults
	  if (!bme280.begin(BME280_I2C_ADDRESS)) {
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
		while (1) {}
	  }
	}

	float sensors_temperature() {
      float temperature = bme280.readTemperature();
      // the BME280 reports temperature in degrees C,
      // but APRS needs it in degrees F,
      // so we perform the appropriate conversion:
      // degreesF = (degreesC * 9 / 5) + 32
      temperature *= 9.0f;
      temperature /= 5.0f;
      temperature += 32.0f;

      return temperature;
	}

	int32_t sensors_pressure() {
    int32_t ReturnValue;
    float pressure;
    pressure = bme280.readPressure();
    ReturnValue = (int32_t)(pressure + 0.5f);
		return ReturnValue;
	}

	float sensors_humidity() {
		return bme280.readHumidity();
	}

#else
	Adafruit_BMP085 bmp805;

	void sensors_setup() {
	  if (!bmp805.begin()) {
		Serial.println("Could not find a valid BMP085 sensor, check wiring!");
		while (1) {}
	  }
	}

	float sensors_temperature() {
		return bmp805.readTemperature();
	}

	int32_t sensors_pressure() {
		return bmp805.readPressure();
	}

	float sensors_humidity() {
		return SHT2x.GetHumidity();
	}
#endif


#endif // ifdef AVR
