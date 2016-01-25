/*								Storage.h
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

class Storage
{
	private:
		char filename[MAX_FILE_SIZE];
		char buffer[MAX_FILE_BUF_SIZE];
		File* pFile;
		
		virtual boolean open();
		virtual boolean close();
	public:	

		virtual uint8_t readln();
		virtual uint8_t writeln(char msg);
};