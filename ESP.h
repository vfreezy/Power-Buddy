/*								ESP.h
!*******************************************************************************
!*	Description: Low level ESP8266 implementation
!*
!*  Known Bugs:  Comments seem to be missing (I was lazy)
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
#include "Common.h"
#include "HardwareSerial.h"
#include "Arduino.h"

class ESP
{
private:
	HardwareSerial *serial;

	void flush();

	String recvString(String target, uint32_t timeout);
	String recvString(String target1, String target2, uint32_t timeout);
	
	uint32_t recvPkg(uint8_t *buffer, uint32_t buffer_size, uint32_t *data_len, uint32_t timeout, uint8_t *coming_mux_id);

	bool recvFind(String target, uint32_t timeout);
	bool recvFindAndFilter(String target, String begin, String end, String &data, uint32_t timeout);
	
	bool eAT();
	bool eATRST();
	bool qATCWMODE(uint8_t *mode);
	bool sATCWMODE(uint8_t mode);
	bool eATGMR(String &version);
	bool sATCWJAP(String ssid, String pwd);
	bool eATCWQAP();
	bool eATCIFSR(String &list, uint8_t type = 0);
	bool sATCIPMUX(uint8_t mode);
	bool sATCIPSERVER(uint8_t mode, uint32_t port = TCP_PORT);
	bool sATCIPSTO(uint32_t timeout);
	bool eATCIPSTATUS(String &list);
	bool sATCIPSENDMultiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
	bool sATCIPCLOSEMulitple(uint8_t mux_id);

public:
	ESP(HardwareSerial &serial, uint32_t baud_rate);

	bool joinAP(String ssid, String pwd);
	bool leaveAP();
	bool restart();
	bool setOprToStationSoftAP();
	bool enableMUX();
	bool startTCPServer(uint32_t port);
	bool setTCPServerTimeout(uint32_t timeout);
	bool stopTCPServer();
	bool send(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
	bool releaseTCP(uint8_t mux_id);

	String getVersion();
	String getLocalIPFull();
	String getLocalIPBrief();
	String getIPStatus();

	uint32_t recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout);
};