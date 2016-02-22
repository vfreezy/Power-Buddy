/*								CommLink.h
!*******************************************************************************
!*	Description: Provides the functionality for providing high level functions
!*				 for interfacing with ESP8266
!*
!*  Known Bugs: Cannot accept more than 1 TCP connection
!*				File Receive is not defined yet
!*
!*
!*	DATE					    COMMENTS							AUTHOR
!*=============================================================================
!* 21FEB16			Initial Delivery							T. von Friesen
!*					
!*
!*****************************************************************************/
/*                              INCLUDES
******************************************************************************/
#pragma once
#include "ESP.h"
#include "SecureDigitalStorage.h"

class CommLink
{
	private:
		//Print data to Log
		void printToLog(String data);

		// Pointer to Log file
		SecureDigitalStorage * pFile_Log = (SecureDigitalStorage *) 0;

		// Pointer to Tx File
		SecureDigitalStorage * pFile_Tx = (SecureDigitalStorage *) 0;

		// Pointer to ESP8266
		ESP * esp8266;

		//Status, true if good, false if bad
		boolean status_ESP8266;
		boolean status_AP;
		boolean status_TCP;

		//TCP Buffer
		uint8_t buffer[MAX_TCP_BUF_SIZE];

		//TCP connection List (only supports 1)
		uint8_t mux_id;
		uint8_t mux_msg_len;

	public:
		CommLink(HardwareSerial &uart);
		CommLink(HardwareSerial &uart, SecureDigitalStorage * pFile);
		~CommLink();

		//Join's the AP with SSID and PWD
		//returns true on success, else false
		boolean connect(String SSID, String PWD);

		//Leaves the connected AP
		//returns true on success, else false
		boolean disconnect();

		//Checks connection status
		//returns true on connect, else false
		boolean isConnected();

		//Starts TCP server
		//returns true on success, else false
		boolean startServer();

		//Stops TCP server
		//returns true on success, else false
		boolean stopServer();

		//Checks for TCP clients
		boolean clientsConnected();

		//Sends pfile over TCP
		//returns true on success, else false
		boolean sendFile(SecureDigitalStorage * pFile);

		//Receives a file over TCP, stores in pFile
		//returns true on success, else false
		boolean rcvFile(SecureDigitalStorage * pFile);

		//Returns the staus of either ESP, AP, or TCP Server
		boolean getStatus(uint8_t TYPE);

		boolean haveClient();
		boolean kickClient();

		uint8_t clientWants();
};
