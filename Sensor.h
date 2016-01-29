/*								Sensor.h
!*******************************************************************************
!*	Description: Provides the base class for sensors. Simple structure to allow
!*				 all sensors to cohere to a standard for easy management.
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
#include "stdint-gcc.h"
/*                          CLASS DEFINITION
******************************************************************************/
class Sensor
{
	protected:
		int pin;
		double LastReadData;
	    
	public:
		//Sensor();
		virtual void read(void);
		double getValue() {return LastReadData;}
};

