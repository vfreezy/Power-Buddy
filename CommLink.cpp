/*								CommLink.cpp
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
#include "CommLink.h"
#include "ESP.h"
#include "Common.h"

CommLink::CommLink(HardwareSerial &uart) {

	this->status_ESP8266 = true;
	this->status_AP = false;
	this->status_TCP = false;
	this->mux_id = 5;

	//Create ESP8266 Object
	this->esp8266 = new ESP(uart, BAUD_RATE);

	//Set to Soft AP mode
	if (!this->esp8266->setOprToStationSoftAP())
		this->status_ESP8266 = false;
}

CommLink::CommLink(HardwareSerial &uart, SecureDigitalStorage * pFile) {

	this->status_ESP8266 = true;
	this->status_AP = false;
	this->status_TCP = false;
	this->mux_id = 5;
	this->pFile_Log = pFile;

	//Create ESP8266 Object
	esp8266 = new ESP(uart, BAUD_RATE);

	//Start Log off with version information
	printToLog(F("ESP8266 established"));
	printToLog(this->esp8266->getVersion());

	//Set to Soft AP mode
	if (!this->esp8266->setOprToStationSoftAP()) {
		printToLog(F("Error Setting softAP"));
		this->status_ESP8266 = false;
	}
	else {
		printToLog(F("Set softAP"));
	}
		
}

CommLink::~CommLink() {
	if (this->pFile_Log)
		printToLog(F("Deleting ESP8266 obj"));
	delete this->esp8266;
}

boolean CommLink::haveClient() {
	memset(this->buffer, 0, MAX_TCP_BUF_SIZE);
	this->mux_msg_len = this->esp8266->recv(&this->mux_id, this->buffer, MAX_TCP_BUF_SIZE-1, BUFFER_TIMEOUT);

	//Terminate the message, makes easier for later cstr stuff
	this->buffer[mux_msg_len] = '\0';
	
	if (this->mux_msg_len > 0) {
		return true;
	}else
		return false;
}

uint8_t CommLink::clientWants() {
	//Read the buffer and determine if there is anything useful

	//Do they want to download something?
	if (strstr((const char*)this->buffer, (const char*)F("g")))
	{
		if (strstr((const char*)this->buffer, (const char*)F("g")))
			return MSG_DL_CFG;
		if (strstr((const char*)this->buffer, (const char*)F("log")))
			return MSG_DL_LOG;
		if (strstr((const char*)this->buffer, (const char*)F("1")))
			return MSG_DL_1;
		if (strstr((const char*)this->buffer, (const char*)F("2")))
			return MSG_DL_2;
	}
	//Do they want to upload something?
	if (strstr((const char*)this->buffer, (const char*)F("upload")))
	{
		if (strstr((const char*)this->buffer, (const char*)F("cfg")))
			return MSG_UL_CFG;
		if (strstr((const char*)this->buffer, (const char*)F("log")))
			return MSG_UL_LOG;
		if (strstr((const char*)this->buffer, (const char*)F("1")))
			return MSG_UL_1;
		if (strstr((const char*)this->buffer, (const char*)F("2")))
			return MSG_UL_2;
	}
	//Do they want to reboot the device?
	if (strstr((const char*)this->buffer, (const char*)F("reboot")))
		return MSG_RST;

	//Unknown command
	return 0x00;
}

boolean CommLink::kickClient() {
	if (this->esp8266->releaseTCP(this->mux_id))
		return true;
	else
		return false;
}

boolean CommLink::getStatus(uint8_t TYPE) {
	switch (TYPE)
	{
	case STATUS_ESP:
		return this->status_ESP8266;
	case STATUS_AP:
		return this->status_AP;
	case STATUS_TCP:
		return this->status_TCP;
	default:
		return false;
	}
}

boolean CommLink::connect(String SSID, String PWD) {
	if (this->esp8266->joinAP(SSID, PWD)) {
		if (this->pFile_Log) {
			printToLog(F("Connected to AP"));
			printToLog(this->esp8266->getLocalIPFull());
		}
		this->status_AP = true;
		return true;
	}
	else {
		if (this->pFile_Log)
			printToLog(F("Failed connection to AP"));
		this->status_AP = false;
		return false;
	}
}

boolean CommLink::disconnect() {
	if (this->esp8266->leaveAP()) {
		if (this->pFile_Log) {
			printToLog(F("Disconnected from AP"));
		}
		this->status_AP = false;
		return true;
	}
	else{
		if (this->pFile_Log) {
			printToLog(F("Unable to disconnect from AP"));
		}
		return false;
	}
}

boolean CommLink::isConnected() { 
	return this->status_AP; 
}

boolean CommLink::startServer() {
	if (this->esp8266->enableMUX()) {
		if (this->pFile_Log) {printToLog(F("MUX Enabled"));}

		if (this->esp8266->startTCPServer(TCP_PORT)){
			if (this->pFile_Log) { printToLog(F("Started TCP Server")); }

			this->status_TCP = true;
			if (this->esp8266->setTCPServerTimeout(TCP_TIMEOUT))
			{
				if (this->pFile_Log) { printToLog(F("Set TCP Server timeout")); }
			}
			else{
				if (this->pFile_Log) { printToLog(F("Failed to set TCP Server timeout")); }
			}
			return true;
		}
		else {
			if (this->pFile_Log) { printToLog(F("Failed to enable TCP Server")); }
			this->status_TCP = false;
			return false;
		}
	}
	else {
		if (this->pFile_Log) { printToLog("Failed to enable MUX"); }
		this->status_TCP = false;
		return false;
	}
}

boolean CommLink::stopServer() {
	if (this->esp8266->stopTCPServer()) {
		if (this->pFile_Log) { printToLog(F("Stopped TCP Server")); }
		this->status_TCP = false;
		return true;
	}
	else {
		if (this->pFile_Log) { printToLog(F("Failed to stop TCP Server")); }
		return false;
	}
}

boolean CommLink::clientsConnected() {

}

boolean CommLink::sendFile(SecureDigitalStorage *pFile) {
	Serial.println(this->mux_id, HEX);
	if (this->mux_id != 0xFF)
	{
		//Save the current file position
		uint32_t oldPos = pFile->getPos();
		//Open the File for Reading
		if (!pFile->ftp_Open())
			return false;
		
		uint8_t len = 0;
		//Read the file until there is not any
		while (pFile->available() > 0){
			memset(this->buffer, 0, MAX_TCP_BUF_SIZE);
			len = pFile->ftp_read(this->buffer, MAX_TCP_BUF_SIZE);
			if (!this->esp8266->send(this->mux_id, this->buffer, len))
				Serial.println("Failed TX");

		}

		//Close the File
		pFile->ftp_Close();

		//Return the file pos to original location
		pFile->setPos(oldPos);

		return true;
	}
	Serial.println("FAILED");
	return false;
}

boolean CommLink::rcvFile(SecureDigitalStorage *pFile) {

}

void CommLink::printToLog(String data) {
	this->pFile_Log->writeln(data.c_str, data.length());
}
