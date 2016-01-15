/*								Voltage.h
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
#pragma once

#include "Sensor.h"

/*                          CLASS DEFINITION
******************************************************************************/
class Voltage : public Sensor
{
	private:
	//Intentionally left blank
	public:
	//Constructor
	Voltage(uint8_t pin);
	
	/*  Reads the analog value on the initialized pin, applies a conversion 
	    equation, and stores that result into lastReadValue                  */
	void read(void);
};