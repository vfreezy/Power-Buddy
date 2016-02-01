/*							SecureDigitalStorage.h
!*******************************************************************************
!*	Description: Provides a wrapper for the SD library to allow an easy object
!*				 oriented approach to manage several files. Allowing the reading
!*				 of files line by line, searching, and writing a line at a time.
!*
!*  Known Bugs: None
!*
!*
!*	DATE					    COMMENTS							AUTHOR
!*=============================================================================
!* 26JAN16			Initial Delivery, have not tested			T. von Friesen
!*					everything yet
!*
!* 01FEB16			Compiled and debugged all functions			T. von Friesen
!*					slight modifications to casting
!*					and fixed a logical error in the
!*					find function.
!*					Added delete file function.
!*
!*****************************************************************************/

/*                              INCLUDES
******************************************************************************/
#pragma once
#include "Common.h"
#include "SD.h"

/*                          CLASS DEFINITION
******************************************************************************/
class SecureDigitalStorage
{
	private:
		uint8_t filename[MAX_FILE_SIZE];			//Filename
		uint8_t buffer[MAX_FILE_BUF_SIZE];			//Read buffer
		uint8_t SS;									//SPI SS pin
		uint32_t currentPos;						//Current file position
		boolean fileOpen;							//File open flag
		File * pFile;								//File pointer
		File fileObj;								//File object
		static boolean sdInt;						//SD card enabled
		
		//Opens the file, returns true on an open file, false on failure
		boolean open(uint8_t mode);//
		
		//Closes the file
		void close();//
		
		//Clear the specified buffer
		void clearBuf(uint8_t * buf, uint8_t len);//
		
		//Check if the SD card is enabled, enables if it is not
		//returns true if it is false if it is not
		boolean isEnabled();//
	public:
		SecureDigitalStorage(uint8_t pin, uint8_t filename[]);//
		
		/*
		Any reading of the file will update the current position to the start
		of the next line. 
		
		Files are not to be held open. They are opened only to perform 1 read / write
		currentPos is constantly updated as a placeholder for subsequent reads.
		
		Writing always appends to the end of the file.
		*/
		
		//Read file until newline or full buffer
		//returns pointer to buffer on success null on failure
		uint8_t * readln();//
		
		//Read file until newline, full buffer, or length
		//returns pointer to buffer on success null on failure
		uint8_t * readln(uint8_t length);
		
		//Read file starting from position until newline, full buffer, or length
		//returns pointer to buffer on success null on failure
		uint8_t * readln(uint8_t length, uint32_t pos);
		
		//Searches file for specified phrase
		//returns position if found, -1 if not found
		uint32_t find(uint8_t phrase[]);
		
		//Writes buffer to file, appends newline at end of line
		//returns number of bytes written
		uint8_t writeln(uint8_t* buf, uint8_t len);
	
		//Deletes the objects associated file
		//returns 0 on failure, 1 on success
		uint8_t deleteFile();
};