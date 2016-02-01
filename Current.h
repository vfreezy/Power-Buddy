/*								Current.h
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
!* 01FEB16			Compiled and debugged, everything			T. von Friesen
!*					checks out, need to test equation
!*					accuracy with live data.
!*
!*****************************************************************************/

/*                              INCLUDES
******************************************************************************/
#pragma once
#include "Sensor.h"
#include "stdint-gcc.h"

/*                          CLASS DEFINITION
******************************************************************************/
class Current : public Sensor
{
	private:
		//Intentionally left blank
	public:
		//Constructor
		Current(int pin);
		
		/*  Reads the analog value on the initialized pin, applies a conversion 
	    equation, and stores that result into lastReadValue                  */
		void read(void);
};