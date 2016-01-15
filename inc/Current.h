/*								Current.h
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