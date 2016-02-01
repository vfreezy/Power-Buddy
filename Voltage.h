/*								Voltage.h
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
!* 01FEB16			Compiled and debugged, everything			T. von Friesen
!*					checks out, need to test equation
!*					accuracy with live data.
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
	Voltage(int pin);
	
	/*  Reads the analog value on the initialized pin, applies a conversion 
	    equation, and stores that result into lastReadValue                  */
	void read(void);
};