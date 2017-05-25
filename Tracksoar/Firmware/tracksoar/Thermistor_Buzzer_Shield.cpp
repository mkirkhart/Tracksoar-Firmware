/**************************************************************************/
/*!
    @file     Thermistor_Buzzer_Shield.cpp
    @author   Michael Kirkhart

*/
/**************************************************************************/

#include "Thermistor_Buzzer_Shield.h"

#define ADC_RESOLUTION    4096L

// calibration coefficients for the Vishay NTCLE100E3103JB0
static const float ThermistorA1 = 3.354016e-3;
static const float ThermistorB1 = 2.569850e-4;
static const float ThermistorC1 = 2.62013e-6;
static const float ThermistorD1 = 6.383091e-8;
static const float ThermistorReferenceResistance = 10000.0;
static const float KelvinsAt0DegreesC = 273.15;

// value of resistor at the bottom of the voltage divider with the thermistor
static const float ThermistorSignalConditioningRL = 20000.0;
static const float ThermistorSignalConditioningRS = 1000.0;
// with a 20K RL and 1K RS, this represents a thermistor resistance of a little more than 2M ohms,
// or a temperature of about -64.9 degrees C
static const uint16_t MinimumThermistorReading = 10;
static const float DefaultLowThermistorTemperature = -99.0;


/**************************************************************************/
/*!
    @brief Constructor
*/
/**************************************************************************/
ThermistorBuzzerShield::ThermistorBuzzerShield() : m_ADC(ADC121C027_ADDR_LOW)
{

}


/**************************************************************************/
/*!
    @brief initializes ADC
*/
/**************************************************************************/
boolean ThermistorBuzzerShield::begin(void)
{
	boolean ReturnValue;

	ReturnValue = m_ADC.begin();

	return ReturnValue;
}


/**************************************************************************/
/*!
    Performs A/D conversion on thermistor interface, computes temperature
	 in degrees C
*/
/**************************************************************************/
boolean ThermistorBuzzerShield::readThermistorTemperatureC(float &temperature)
{
	boolean ReturnValue = true;

	uint16_t RawThermistorReading;

	RawThermistorReading = m_ADC.ReadConversionResults();

	// make sure we do not have a missing thermistor
	if(RawThermistorReading > MinimumThermistorReading)
	{
		float ThermistorResistance;
		float NaturalLogOfThermistorResistanceToReferenceResistance;
		float denominator;

		// compute the thermistor resistance
		ThermistorResistance = (float)(ADC_RESOLUTION) / (float)RawThermistorReading;
		ThermistorResistance -= 1.0;
		ThermistorResistance *= ThermistorSignalConditioningRL;
		ThermistorResistance -= ThermistorSignalConditioningRS;

		// compute the thermistor temperature (in Kelvins)
		NaturalLogOfThermistorResistanceToReferenceResistance = log(ThermistorResistance / ThermistorReferenceResistance);

		denominator = ThermistorA1;
		denominator += ThermistorB1 * NaturalLogOfThermistorResistanceToReferenceResistance;
		denominator += ThermistorC1 * NaturalLogOfThermistorResistanceToReferenceResistance * NaturalLogOfThermistorResistanceToReferenceResistance;
		denominator += ThermistorD1 * NaturalLogOfThermistorResistanceToReferenceResistance * NaturalLogOfThermistorResistanceToReferenceResistance * NaturalLogOfThermistorResistanceToReferenceResistance;

		temperature = 1.0 / denominator;
		// Finally, convert from Kelvins to degrees C
		temperature -= KelvinsAt0DegreesC;
	}
	else
	{
		temperature = DefaultLowThermistorTemperature;
	}

	return ReturnValue;
}


/**************************************************************************/
/*!
    Performs A/D conversion on thermistor interface, computes temperature
	 in degrees C
*/
/**************************************************************************/
boolean ThermistorBuzzerShield::readThermistorTemperatureF(float &temperature)
{
	boolean ReturnValue = true;

	// read from thermistor, compute temperature in degrees C
	ReturnValue = readThermistorTemperatureC(temperature);

	if(ReturnValue)
	{
		// convert from degrees C to degrees F
		temperature *= 9.0f;
		temperature /= 5.0f;
		temperature += 32.0f;
	}

	return ReturnValue;
}


