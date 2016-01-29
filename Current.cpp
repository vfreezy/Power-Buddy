/*								Current.cpp
!*******************************************************************************
!*	Description: Provides the functionality for current sensors.
!*
!*  Known Bugs: None
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
#include "Current.h"
#include "Sensor.h"
#include "Arduino.h"
#include "stdint-gcc.h"



Current::Current(int pin)
{
	this->pin = pin;
	this->LastReadData = 0;
}

void Current::read(void)
{
	double temp = 0;
	//Read the analog value on pin
	temp = analogRead(this->pin);
	//Analog read returns a value of 1024 resolution => 1 unit = .0049V
	
	/*	Current Sensing Conversion Equation:
		0.5 V = -30A
		2.5 V =   0A
		4.5 V = +30A
						Equation: 
			Vout = Iin ( 2 / 30) + 2.5
			Iin = ( Vout - 2.5 ) * 15
	*/
	temp = ((temp * .0049) - 2.5) * 15;

	//Cast the double value into an integer
	this->LastReadData = (uint8_t) temp;
}