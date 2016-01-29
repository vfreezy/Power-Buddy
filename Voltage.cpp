/*								Voltage.cpp
!*******************************************************************************
!*	Description: Provides the functionality for voltage sensors.
!*
!*  Known Bugs: Equation for voltage not implemented
!*
!*
!*	DATE					    COMMENTS							AUTHOR
!*=============================================================================
!* 26JAN16			Initial Delivery, have not tested			T. von Friesen
!*					everything yet
!*
!*****************************************************************************/

/*                              INCLUDES
******************************************************************************/
#include "Voltage.h"
#include "Arduino.h"
#include "stdint-gcc.h"


Voltage::Voltage(int pin)
{
	this->pin = pin;
	this->LastReadData = 0;
}

void Voltage::read(void)
{
	double temp = 0;
	//Read the analog value on pin
	temp = analogRead(this->pin);
	
	//Analog read returns a value of 1024 resolution => 1 unit = .0049V
	
	/*	Voltage Sensing Conversion Equation:
		Vout = 0.0281Vin - 0.3427
		Vin = (Vout + 0.3427)/0.0281
	*/
	temp = ((temp + 0.3427) * .0049) / 0.0281;
	//Cast the double value into an integer
	this->LastReadData = (uint8_t) temp;
}