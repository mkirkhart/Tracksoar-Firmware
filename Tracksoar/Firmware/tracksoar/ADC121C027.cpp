/**************************************************************************/
/*!
    @file     ADC121C027.cpp
    @author   Michael Kirkhart

*/
/**************************************************************************/

#include "ADC121C027.h"


/**************************************************************************/
/*!
    @brief constructor
*/
/**************************************************************************/
ADC121C027::ADC121C027(const uint8_t addr)
{
	m_addr = addr;
}


/**************************************************************************/
/*!
    @brief Initializes I2C and configures the converter
*/
/**************************************************************************/
boolean ADC121C027::begin(void)
{
	boolean ReturnValue = true;

	// initialize the Wire library
	Wire.begin();

	// set configuration register as follows:
	// Automatic conversion mode disabled
	// Alert hold disabled
	// Alert flag disabled
	uint8_t Configuration = ADC121C027_AUTO_MODE_DISABLED;
	write8(ADC121C027_CONFIGURATION, Configuration);

	return ReturnValue;
}


/**************************************************************************/
/*!
    @brief Performs conversion, returns conversion results
*/
/**************************************************************************/
uint16_t ADC121C027::ReadConversionResults(void)
{
	uint16_t Results;

	// in the case of non-automatic (normal) mode, a new conversion is
	// triggered when the conversion results register is read.
	// Therefore, we need to read the register to start a conversion,
	// wait until the conversion is complete, and then read the conversion
	// results register again to get the results

	// Dummy read to start a conversion
	Results = read16(ADC121C027_CONVERSION_RESULTS);
	// wait ...

	// Now we can read the results of the conversion we started
	Results = read16(ADC121C027_CONVERSION_RESULTS);
	// mask out everything except the actual conversion results
	Results &= ADC121C027_CONVERSION_RESULTS_RESULT_MASK;

	return Results;
}


/**************************************************************************/
/*!
    @brief  Sets address register pointer
*/
/**************************************************************************/

void ADC121C027::setAddressPointerRegister(const ADC121C027Registers_t reg)
{
	Wire.beginTransmission(m_addr);
#if ARDUINO >= 100
	Wire.write(reg);
#else
	Wire.send(reg);
#endif
}


/**************************************************************************/
/*!
    @brief  Writes a register and an 8 bit value over I2C
*/
/**************************************************************************/
void ADC121C027::write8(const ADC121C027Registers_t reg, const uint8_t value)
{
	setAddressPointerRegister(reg);
#if ARDUINO >= 100
	Wire.write(value);
#else
	Wire.send(value);
#endif
	Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Writes a register and a 16 bit value over I2C
*/
/**************************************************************************/
void ADC121C027::write16(const ADC121C027Registers_t reg, const uint16_t value)
{
	uint8_t Temp;

	setAddressPointerRegister(reg);

	// NOTE: The converter expects 16 bit values to be written in big endian order
#if ARDUINO >= 100
	Temp = value >> 8;
	Wire.write(Temp);
	Temp = value & 0xff;
	Wire.write(Temp);
#else
	Temp = value >> 8;
	Wire.send(Temp);
	Temp = value & 0xff;
	Wire.send(Temp);
#endif
	Wire.endTransmission();
}


/**************************************************************************/
/*!
    @brief  Reads an 8 bit value over I2C
*/
/**************************************************************************/
uint8_t ADC121C027::read8(const ADC121C027Registers_t reg)
{
	uint8_t ReadResults = 0;

	setAddressPointerRegister(reg);

	Wire.requestFrom(m_addr, sizeof(ReadResults));
#if ARDUINO >= 100
	ReadResults = Wire.read();
#else
	ReadResults = Wire.receive();
#endif

	return ReadResults;
}

/**************************************************************************/
/*!
    @brief  Reads a 16 bit values over I2C
*/
/**************************************************************************/
uint16_t ADC121C027::read16(const ADC121C027Registers_t reg)
{
	uint16_t ReadResults;

	Wire.beginTransmission(m_addr);
#if ARDUINO >= 100
	Wire.write(reg);
#else
	Wire.send(reg);
#endif
	Wire.endTransmission();

	Wire.requestFrom(m_addr, sizeof(ReadResults));

	// NOTE: 16 bit values are sent by the converter in big endian order
#if ARDUINO >= 100
	ReadResults = Wire.read();
	ReadResults <<= 8;
	ReadResults |= Wire.read();
#else
	ReadResults = Wire.receive();
	ReadResults <<= 8;
	ReadResults |= Wire.receive();
#endif

	return ReadResults;
}


