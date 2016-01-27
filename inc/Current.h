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
!*****************************************************************************/

/*                              INCLUDES
******************************************************************************/
#pragma once
#include "Sensor.h"

/*                          CLASS DEFINITION
******************************************************************************/
class Current : public Sensor
{
	private:
		//Intentionally left blank
	public:
		//Constructor
		Current(uint8_t pin);
		
		/*  Reads the analog value on the initialized pin, applies a conversion 
	    equation, and stores that result into lastReadValue                  */
		void read(void);
};