/*								Voltage.cpp
!*******************************************************************************
!*
!*
!*
!*
!*
!*
!*
!*
!*
!*****************************************************************************/

/*                              INCLUDES
******************************************************************************/
#include "Voltage.h"



Voltage::Voltage(uint8_t pin)
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
		
	*/
	
	//Cast the double value into an integer
	this->LastReadData = (uint8_t) temp;
}