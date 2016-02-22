/*								Common.h
!*******************************************************************************
!*	Description: Provides a location for definitions to be housed allowing easy
!*				 editorial abilities!
!*
!*  Known Bugs: None
!*
!*
!*	DATE					    COMMENTS							AUTHOR
!*=============================================================================
!* 26JAN16			Initial Delivery, have not tested			T. von Friesen
!*					everything yet
!*
!* 01FEB16			Added debug define, this puts the			T. von Friesen
!*					program in verbose mode over serial.
!*
!* 21FEB16			Restructured layout							T. von Friesen
!*					Added CommLink Defines
!*
!*****************************************************************************/
#pragma once
/*								Debug
******************************************************************************/
#define VERBOSE

/*                         Pin Assignments
******************************************************************************/
#define PIN_CURRENT_1			1				//Analog pin for Current
#define PIN_CURRENT_2			2				//Analog pin for Currrent
#define PIN_VOLTAGE_1			3				//Analog pin for Voltage
#define PIN_SD_SS				53				//SPI Chip select for SD
#define PIN_ESP_0				3				//Set low for Flash
#define PIN_ESP_2				2				//Required to be set high

/*                         SecureDigitalStorage
******************************************************************************/
#define MAX_FILE_SIZE			11				//Supports 7.3 Filenaming
#define MAX_FILE_BUF_SIZE		100				//File read buffer

/*                              CommLink
******************************************************************************/
#define TCP_TIMEOUT				1000			//TCP Server connection timeout
#define FIND_TIMEOUT			1000			//Receive Find timeout
#define FIND_OK_TIMEOUT			10000
#define TCP_PORT				333				//TCP Server port
#define BAUD_RATE				115200			//Serial comm rate
#define MAX_TCP_BUF_SIZE		10				//TCP Server buffer
#define COMMAND_TIMEOUT			1000			//AT Command Timeout
#define BUFFER_TIMEOUT			500				//TCP buffer read timeout	
#define STATUS_ESP				0				
#define STATUS_AP				1
#define	STATUS_TCP				2
//							Message Traffic
#define MSG_DL_CFG				0xF0
#define MSG_DL_LOG				0xF1
#define MSG_DL_1				0xF2
#define MSG_DL_2				0xF3
#define MSG_UL_CFG				0xF4
#define MSG_UL_LOG				0xF5
#define MSG_UL_1				0xF6
#define MSG_UL_2				0xF7
#define MSG_RST					0xF8