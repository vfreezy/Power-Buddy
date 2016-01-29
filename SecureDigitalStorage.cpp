/*						SecureDigitalStorage.cpp
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
!*****************************************************************************/

/*                              INCLUDES
******************************************************************************/
#include "SecureDigitalStorage.h"
#include "Common.h"
#include "SD.h"
#include "string.h"
#include "stdio.h"

boolean SecureDigitalStorage::sdInt = false;

SecureDigitalStorage::SecureDigitalStorage(uint8_t pin, uint8_t filename[])
{
	#ifdef DEBUG
		Serial.print("Creating Object\n Pin = ");
		Serial.print((int) pin,DEC);
		Serial.print(" Filename = ");
		Serial.println((char *)filename);
	#endif
	//Copy filename over, there is no validation here perhaps add it later?
	// filename needs to be in the format of 7.3 (xxxxxxx.xxx)
	for (int x = 0; x < MAX_FILE_SIZE; x++)
		this->filename[x] = filename[x];
	
	//Set SPI SS pin
	this->SS = pin;
	
	//Set variables to defaults
	this->fileOpen = false;
	this->currentPos = 0;
	this->pFile = NULL;
	
	//Clear buffers
	clearBuf(&this->buffer[0], MAX_FILE_BUF_SIZE);
	
	//Enable the Shield, the boolean value is disregarded here
	// maybe the true false can be used?
	isEnabled();
}
boolean SecureDigitalStorage::isEnabled() 
{
	//Determine if SD shield is currently configured
	if (!this->sdInt)
	{
	//SD Shield is not configured
	#ifdef DEBUG
		Serial.print("SD !Enabled, sdInt = ");
		Serial.println(this->sdInt, HEX);
	#endif
		if (SD.begin(this->SS))
		{
			//SD Shield configured
			this->sdInt = true;
			
			#ifdef DEBUG
				Serial.print("SD Enabled, sdInt = ");
				Serial.println(this->sdInt, HEX);
			#endif
			//Everything checks out, let 'em know!
			return true;
		}
		//An error occurred at some point
		#ifdef DEBUG
			Serial.print("SD failed Enable, sdInt = ");
			Serial.println(this->sdInt, HEX);
		#endif
		return false;
	}
	//SD shield is already enabled, silly goose!
	return true;
}
boolean SecureDigitalStorage::open(uint8_t mode)
{
	//Check for silliness of opening an already open file!
	if (this->fileOpen) {return true;}
		
	//Check to see if the file exists
	if (SD.exists((char *)this->filename))
	{
		//Open the file
		this->fileObj = SD.open((char *)this->filename, mode);
		this->pFile = & this->fileObj;
		//Verify the file is indeed open
		if(*this->pFile)
		{
			//Hurray!
			this->fileOpen = true;
			return true;
		}
		// =(
		return false;
	}else
	{
		//File does not exist, create the file. However if the mode is read
		// the file must be opened in write mode then switch to read mode
		this->fileObj = SD.open((char *) this->filename, FILE_WRITE);
		this->pFile = & this->fileObj;
		//Verify the file is indeed open
		if (*this->pFile)
		{
			//Hurray!
			if (mode == FILE_WRITE)
			{
				this->fileOpen = true;
				return true;
			}else
			{
				this->pFile->close();
				
				//Reopen file in read mode
				this->fileObj = SD.open((char *) this->filename, mode);
				this->pFile = & this->fileObj;
				//Verify the file is indeed open
				if(*this->pFile)
				{
					//Hurray!
					this->fileOpen = true;
					return true;
				}
				// =(
				return false;
			}	
		}
		// =(
		return false;
	}
	
}
void SecureDigitalStorage::close()
{
	this->pFile->close();
	this->fileOpen = false;
}
void SecureDigitalStorage::clearBuf(uint8_t * buf, uint8_t len)
{
	memset(buf, 0, len);
}
uint8_t * SecureDigitalStorage::readln()
{
	//Open the file for reading! WE ALL LOVE READING SHIT!
	if (open(FILE_READ))
	{
		this->pFile->seek(this->currentPos);
		//Well the file is opened at this point, is there anything to read?
		if (this->pFile->available())
		{
			//Start with a fresh buffer!
			clearBuf(&this->buffer[0], MAX_FILE_BUF_SIZE);

			//Here is the juicy part where we read the good stuff
			uint8_t readbyte;	
			for (int x = 0; x < (MAX_FILE_BUF_SIZE - 1); x++)
			{
				//Now we shall read each byte!
				//Is there any more bytes (-1) means no more =(
				if ((readbyte =	this->pFile->read()))
				{
					if ((this->buffer[x] = readbyte) == '\n')
					{
						//NEWLINE!
						this->buffer[x] = '\0';
						this->currentPos = this->pFile->position();
						close();
						return & this->buffer[0];
					}
					this->currentPos = this->pFile->position();
				}else
				{
					//We have reached the end of the file, with no newline
					// and the buffer is not filled yet, so we need to terminate
					// the buffer AND break outa this B!
					this->buffer[x] = '\0';
					this->currentPos = 0;
					close();
					return & this->buffer[0];
				}
			}
			//If you get to this point, you did not reach the EOF or NL or Fill the
			// buffer, so terminate the string and send it on down the line!
			this->buffer[MAX_FILE_BUF_SIZE-1] = '\0';
			this->currentPos = this->pFile->position();
			close();
			return & this->buffer[0];
		}else
		{
			//LOL you opened an empty file to read... el stupido
			//...
			//Here let me help you close your empty file.
			//You might have actually read all the file, in that case.. sorry!
			this->currentPos = 0;
			close();
			return NULL;
		}
	}else
	{
		//Unfortunately your file is magical and cannot be opened sorry =(
		return NULL;
	}	
}
uint8_t * SecureDigitalStorage::readln(uint8_t length)
{
	//Open the file for reading! WE ALL LOVE READING SHIT!
	if (open(FILE_READ))
	{
		this->pFile->seek(this->currentPos);
		//Well the file is opened at this point, is there anything to read?
		if (this->pFile->available())
		{
			//Start with a fresh buffer!
			clearBuf(&this->buffer[0], MAX_FILE_BUF_SIZE);

			//Here is the juicy part where we read the good stuff
			uint8_t readbyte;
			for (int x = 0; x < (length - 1); x++)
			{
				//Now we shall read each byte!
				//Is there any more bytes (-1) means no more =(
				if ((readbyte =	this->pFile->read()))
				{
					if ((this->buffer[x] = readbyte) == '\n')
					{
						//NEWLINE!
						this->buffer[x] = '\0';
						this->currentPos = this->pFile->position();
						close();
						return & this->buffer[0];
					}
					this->currentPos = this->pFile->position();
				}else
				{
					//We have reached the end of the file, with no newline
					// and the buffer is not filled yet, so we need to terminate
					// the buffer AND break outa this B!
					this->buffer[x] = '\0';
					this->currentPos = 0;
					close();
					return & this->buffer[0];
				}
			}
			//If you get to this point, you did not reach the EOF or NL or Fill the
			// buffer, so terminate the string and send it on down the line!
			this->buffer[length-1] = '\0';
			this->currentPos = this->pFile->position();
			close();
			return & this->buffer[0];
		}else
		{
			//LOL you opened an empty file to read... el stupido
			//...
			//Here let me help you close your empty file.
			//You might have actually read all the file, in that case.. sorry!
			this->currentPos = 0;		
			close();
			return NULL;
		}
	}else
	{
		//Unfortunately your file is magical and cannot be opened sorry =(
		return NULL;
	}
}
uint8_t * SecureDigitalStorage::readln(uint8_t length, uint32_t pos)
{
	//Open the file for reading! WE ALL LOVE READING SHIT!
	if (open(FILE_READ))
	{
		this->pFile->seek(this->currentPos);
		//Well the file is opened at this point, is there anything to read?
		if (this->pFile->available())
		{
			//Are you clairvoyant?!
			if (pos >= this->pFile->size())
			{
				//What are the next POWERBALL NUMBERS?>!?!?!
				close();
				return NULL;
			}

			//Start with a fresh buffer!
			clearBuf(&this->buffer[0], MAX_FILE_BUF_SIZE);

			//Here is the juicy part where we read the good stuff
			uint8_t readbyte;
			for (int x = 0; x < (length - 1); x++)
			{
				//Now we shall read each byte!
				//Is there any more bytes (-1) means no more =(
				if ((readbyte =	this->pFile->read()))
				{
					if ((this->buffer[x] = readbyte) == '\n')
					{
						//NEWLINE!
						this->buffer[x] = '\0';
						this->currentPos = this->pFile->position();
						close();
						return & this->buffer[0];
					}
					this->currentPos = this->pFile->position();
				}else
				{
					//We have reached the end of the file, with no newline
					// and the buffer is not filled yet, so we need to terminate
					// the buffer AND break outa this B!
					this->buffer[x] = '\0';
					this->currentPos = 0;
					close();
					return & this->buffer[0];
				}
			}
			//If you get to this point, you did not reach the EOF or NL or Fill the
			// buffer, so terminate the string and send it on down the line!
			this->buffer[length-1] = '\0';
			this->currentPos = this->pFile->position();
			close();
			return & this->buffer[0];
		}else
		{
			//LOL you opened an empty file to read... el stupido
			//...
			//Here let me help you close your empty file.
			//You might have actually read all the file, in that case.. sorry!
			this->currentPos = 0;
			close();
			return NULL;
		}
	}else
	{
		//Unfortunately your file is magical and cannot be opened sorry =(
		return NULL;
	}	
}
uint32_t SecureDigitalStorage::find(uint8_t phrase[])
{
	//Lets make sure we keep track of our position before we it becomes foobar
	uint32_t posHolder = this->currentPos;
	uint32_t foundItAt = 0;
	uint8_t * temp;
	uint8_t * found;
	//Start at the beginning of the file!
	this->currentPos = 0;
	do 
	{
		//Fill the buffer with the next line!
		temp = readln();

		//Check to see if the phrase is in the read line
		if (!(found = (uint8_t *)strstr((char *)&this->buffer[0], (char *) &phrase[0])))
		{
			//FOUND IT!
			//Compare addresses until we find out exactly how far into the string it is
			for (int x = 0; x < strlen((char *)&this->buffer[0]); x++)
			{
				if (&this->buffer[0] == found)
				{
					foundItAt = x;
					break;
				}
			}
			//Last position - length of read line + where it was on the previous line
			foundItAt = this->currentPos - (strlen((char*)&this->buffer[0]) + foundItAt);

			//Putting things back the way we found it
			this->currentPos = posHolder;
			return foundItAt;
		}
	} while (temp != NULL);

	//Putting things back the way we found it
	this->currentPos = posHolder;

	//You didnt find anything, sucker!
	return 0;
}
uint8_t SecureDigitalStorage::writeln(uint8_t * buf, uint8_t len)
{
	uint8_t byteswritten = 0;
	//Lets make sure we can open the file!
	if (open(FILE_WRITE))
	{

		//Ok we have an opened file lets make sure there is a newline
		// at the end of the buffer, if not add one!
		if (buf[len-1] != '\n')
		{
			//Alright, there is not a newline at the end, lets make sure
			// we wont overflow our buffer!
			if (len == MAX_FILE_BUF_SIZE)
			{
				//Good catch! lets just change that last character
				// since the operator is trying to do some slick shit
				buf[len-1] = '\n';
			}else
			{
				//No buffer overflow threat, lets just take a little
				// newline character and place it gently at the end!
				// shhhhh... no one will notice!
				buf[len++] = '\n';
			}
			//Sweet, our buffer is ready to be written to the file!
			// LETS DO THIS!
			// WAIT FOR IT!
			byteswritten = this->pFile->write(buf, len);
			//Crazy complicated, I know...
			//Lets make sure we CTRL+S our work and bail outa here!
			close();
			return byteswritten;
		}

	}else
	{
		//Unfortunately your file is magical and cannot be opened sorry =(
		return byteswritten;
	}
}