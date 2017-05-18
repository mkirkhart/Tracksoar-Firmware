/**************************************************************************/
/*!
    @file     Thermistor_Buzzer_Shield.h
    @author   Michael Kirkhart

*/
/**************************************************************************/
#ifndef _THERMISTOR_BUZZER_SHIELD_H_
#define _THERMISTOR_BUZZER_SHIELD_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
#include <Wire.h>

#include "ADC121C027.h"

class ThermistorBuzzerShield
{
	public:
		ThermistorBuzzerShield();
		boolean begin(void);

	public:
		boolean readThermistorTemperatureC(float &temperature);
		boolean readThermistorTemperatureF(float &temperature);

	private:
		ADC121C027 m_ADC;
};

#endif

