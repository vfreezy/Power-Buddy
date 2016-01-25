/*							SecureDigitalStorage.h
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
#include "Common.h"
#include "SD.h"

/*                          CLASS DEFINITION
******************************************************************************/
class SecureDigitalStorage : public Storage
{
	private:
		uint8_t SS;
		
		boolean open();
		boolean close();
		
	public:
		SecureDigitalStorage();
		uint8_t readln();
		uint8_t writeln(uint8_t msg);
		
		uint8_t setFile(uint8_t fn);
		uint8_t getData();
	
};