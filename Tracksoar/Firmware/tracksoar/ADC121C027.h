/**************************************************************************/
/*!
    @file     ADC121C027.h
    @author   Michael Kirkhart

*/
/**************************************************************************/
#ifndef _ADC121C027_H_
#define _ADC121C027_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <Wire.h>

// I2C address options
#define ADC121C027_ADDR_FLOATING  (0x50)
#define ADC121C027_ADDR_LOW       (0x51)
#define ADC121C027_ADDR_HIGH      (0x52)

// Conversion results register defines
#define ADC121C027_CONVERSION_RESULTS_ALERT_FLAG  (0x8000)
#define ADC121C027_CONVERSION_RESULTS_RESULT_MASK (0x0fff)

// Alert status register defines
#define ADC121C027_ALERT_STATUS_OVERRANGE_FLAG    (0x02)
#define ADC121C027_ALERT_STATUS_UNDERRANGE_FLAG   (0x01)

// Configuration register defines/enums
typedef enum
{
	ADC121C027_AUTO_MODE_DISABLED = 0,
	ADC121C027_TCONVERT_32        = (1 << 5),
	ADC121C027_TCONVERT_64        = (2 << 5),
	ADC121C027_TCONVERT_128       = (3 << 5),
	ADC121C027_TCONVERT_256       = (4 << 5),
	ADC121C027_TCONVERT_512       = (5 << 5),
	ADC121C027_TCONVERT_1024      = (6 << 5),
	ADC121C027_TCONVERT_2048      = (7 << 5)
}	ADC121C027ConfigurationCycleTime_t;

#define ADC121C027_CONFIGURATION_ALERT_HOLD			(0x10)
#define ADC121C027_CONFIGURATION_ALERT_FLAG_ENABLE	(0x08)

// Register definitions
typedef enum
{
	ADC121C027_CONVERSION_RESULTS	= 0,	// 16 bits wide
	ADC121C027_ALERT_STATUS       = 1,	// 8 bits wide
	ADC121C027_CONFIGURATION      = 2,	// 8 bits wide
	ADC121C027_LOW_LIMIT          = 3,	// 16 bits wide
	ADC121C027_HIGH_LIMIT         = 4,	// 16 bits wide
	ADC121C027_HYSTERESIS         = 5,	// 16 bits wide
	ADC121C027_LOWEST_CONVERSION  = 6,	// 16 bits wide
	ADC121C027_HIGHEST_CONVERSION = 7	// 16 bits wide
}	ADC121C027Registers_t;


class ADC121C027
{
	// constructor, initialization functions
	public:
		ADC121C027(const uint8_t addr = ADC121C027_ADDR_LOW);
		boolean begin(void);

	// public member functions
	public:
		uint16_t ReadConversionResults(void);

	// private member functions
	private:
		void setAddressPointerRegister(const ADC121C027Registers_t reg);
		void write8(const ADC121C027Registers_t reg, const uint8_t value);
		void write16(const ADC121C027Registers_t reg, const uint16_t value);
		uint8_t read8(const ADC121C027Registers_t reg);
		uint16_t read16(const ADC121C027Registers_t reg);

	// private data members
	private:
		uint8_t m_addr;

};
#endif

