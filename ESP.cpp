/*								ESP.cpp
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
#include "ESP.h"
#include "Common.h"

ESP::ESP(HardwareSerial &serial, uint32_t baud_rate) {
	this->serial = &serial;
	this->serial->begin(baud_rate);
	flush();
}

bool ESP::joinAP(String ssid, String pwd)
{
	return sATCWJAP(ssid, pwd);
}

bool ESP::leaveAP(void)
{
	return eATCWQAP();
}

String ESP::getLocalIPFull(void)
{
	String list;
	eATCIFSR(list);
	return list;
}

String ESP::getLocalIPBrief(void)
{
	String list;
	eATCIFSR(list, 1);
	return list;
}

String ESP::getIPStatus(void)
{
	String list;
	eATCIPSTATUS(list);
	return list;
}

bool ESP::setOprToStationSoftAP(void)
{
	uint8_t mode;
	if (!qATCWMODE(&mode)) {
		return false;
	}
	if (mode == 3) {
		return true;
	}
	else {
		if (sATCWMODE(3) && restart()) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool ESP::restart(void)
{
	unsigned long start;
	if (eATRST()) {
		delay(2000);
		start = millis();
		while (millis() - start < 3000) {
			if (eAT()) {
				delay(1500); /* Waiting for stable */
				return true;
			}
			delay(100);
		}
	}
	return false;
}

bool ESP::enableMUX(void)
{
	return sATCIPMUX(1);
}

bool ESP::startTCPServer(uint32_t port)
{
	if (sATCIPSERVER(1, port)) {
		return true;
	}
	return false;
}

bool ESP::setTCPServerTimeout(uint32_t timeout)
{
	return sATCIPSTO(timeout);
}

bool ESP::stopTCPServer(void)
{
	sATCIPSERVER(0);
	restart();
	return false;
}

bool ESP::releaseTCP(uint8_t mux_id)
{
	return sATCIPCLOSEMulitple(mux_id);
}

void ESP::flush() {
	//Serial.flush() waits for outgoing data transmission to complete
	//	prior to flushing the buffer, we do not want this
	while (this->serial->available())
		this->serial->read();
}

String ESP::getVersion(void)
{
	String version;
	eATGMR(version);
	return version;
}

bool ESP::send(uint8_t mux_id, const uint8_t *buffer, uint32_t len)
{
	return sATCIPSENDMultiple(mux_id, buffer, len);
}

uint32_t ESP::recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout)
{
	return recvPkg(buffer, buffer_size, NULL, timeout, coming_mux_id);
}

uint32_t ESP::recvPkg(uint8_t *buffer, uint32_t buffer_size, uint32_t *data_len, uint32_t timeout, uint8_t *coming_mux_id)
{
	String data;
	char a;
	int32_t index_PIPDcomma = -1;
	int32_t index_colon = -1; /* : */
	int32_t index_comma = -1; /* , */
	int32_t len = -1;
	int8_t id = -1;
	bool has_data = false;
	uint32_t ret;
	unsigned long start;
	uint32_t i;

	if (buffer == NULL) {
		return 0;
	}

	start = millis();
	while (millis() - start < timeout) {
		if (this->serial->available() > 0) {
			a = this->serial->read();
			data += a;
		}
		index_PIPDcomma = data.indexOf(F("+IPD,"));
		if (index_PIPDcomma != -1) {
			index_colon = data.indexOf(':', index_PIPDcomma + 5);
			if (index_colon != -1) {
				index_comma = data.indexOf(',', index_PIPDcomma + 5);
				/* +IPD,id,len:data */
				if (index_comma != -1 && index_comma < index_colon) {
					id = data.substring(index_PIPDcomma + 5, index_comma).toInt();
					if (id < 0 || id > 4) {
						return 0;
					}
					len = data.substring(index_comma + 1, index_colon).toInt();
					if (len <= 0) {
						return 0;
					}
				}
				else { /* +IPD,len:data */
					len = data.substring(index_PIPDcomma + 5, index_colon).toInt();
					if (len <= 0) {
						return 0;
					}
				}
				has_data = true;
				break;
			}
		}
	}

	if (has_data) {
		i = 0;
		ret = len > buffer_size ? buffer_size : len;
		start = millis();
		while (millis() - start < 3000) {
			while (this->serial->available() > 0 && i < ret) {
				a = this->serial->read();
				buffer[i++] = a;
			}
			if (i == ret) {
				flush();
				if (data_len) {
					*data_len = len;
				}
				if (index_comma != -1 && coming_mux_id) {
					*coming_mux_id = id;
				}
				return ret;
			}
		}
	}
	return 0;
}

String ESP::recvString(String target, uint32_t timeout)
{
	String data;
	char a;
	unsigned long start = millis();
	while (millis() - start < timeout) {
		while (this->serial->available() > 0) {
			a = this->serial->read();
			if (a == '\0') continue;
			data += a;
		}
		if (data.indexOf(target) != -1) {
			break;
		}
	}
	return data;
}

String ESP::recvString(String target1, String target2, uint32_t timeout)
{
	String data;
	char a;
	unsigned long start = millis();
	while (millis() - start < timeout) {
		while (this->serial->available() > 0) {
			a = this->serial->read();
			if (a == '\0') continue;
			data += a;
		}
		if (data.indexOf(target1) != -1) {
			break;
		}
		else if (data.indexOf(target2) != -1) {
			break;
		}
	}
	return data;
}

bool ESP::recvFind(String target, uint32_t timeout)
{
	String data_tmp;
	data_tmp = recvString(target, timeout);
	if (data_tmp.indexOf(target) != -1) {
		return true;
	}
	return false;
}

bool ESP::recvFindAndFilter(String target, String begin, String end, String &data, uint32_t timeout)
{
	String data_tmp;
	data_tmp = recvString(target, timeout);
	if (data_tmp.indexOf(target) != -1) {
		int32_t index1 = data_tmp.indexOf(begin);
		int32_t index2 = data_tmp.indexOf(end);
		if (index1 != -1 && index2 != -1) {
			index1 += begin.length();
			data = data_tmp.substring(index1, index2);
			return true;
		}
	}
	data = "";
	return false;
}

bool ESP::qATCWMODE(uint8_t *mode)
{
	String str_mode;
	bool ret;
	if (!mode) {
		return false;
	}
	flush();
	this->serial->println(F("AT+CWMODE?"));
	ret = recvFindAndFilter(F("OK"), F("+CWMODE:"), F("\r\n\r\nOK"), str_mode, COMMAND_TIMEOUT);
	if (ret) {
		*mode = (uint8_t)str_mode.toInt();
		return true;
	}
	else {
		return false;
	}
}

bool ESP::sATCWMODE(uint8_t mode)
{
	String data;
	flush();
	this->serial->print(F("AT+CWMODE="));
	this->serial->println(mode);

	data = recvString(F("OK"), F("no change"), COMMAND_TIMEOUT);
	if (data.indexOf("OK") != -1 || data.indexOf("no change") != -1) {
		return true;
	}
	return false;
}

bool ESP::eAT(void)
{
	flush();
	this->serial->println(F("AT"));
	return recvFind(F("OK"), COMMAND_TIMEOUT);
}

bool ESP::eATRST(void)
{
	flush();
	this->serial->println(F("AT+RST"));
	return recvFind(F("OK"), COMMAND_TIMEOUT);
}

bool ESP::eATGMR(String &version)
{
	flush();
	this->serial->println(F("AT+GMR"));
	return recvFindAndFilter(F("OK"), F("\r\r\n"), F("\r\n\r\nOK"), version, COMMAND_TIMEOUT);
}

bool ESP::sATCWJAP(String ssid, String pwd)
{
	String data;
	flush();
	this->serial->print(F("AT+CWJAP=\""));
	this->serial->print(ssid);
	this->serial->print("\",\"");
	this->serial->print(pwd);
	this->serial->println("\"");

	data = recvString(F("OK"), F("FAIL"), 10000);
	if (data.indexOf("OK") != -1) {
		return true;
	}
	return false;
}

bool ESP::eATCWQAP(void)
{
	String data;
	flush();
	this->serial->println(F("AT+CWQAP"));
	return recvFind(F("OK"),COMMAND_TIMEOUT);
}

bool ESP::eATCIFSR(String &list, uint8_t type)
{
	flush();
	this->serial->println(F("AT+CIFSR"));
	if (type)
		return recvFindAndFilter(F("OK"), F("STAIP,"), F("+CIFSR:STAMAC"), list, COMMAND_TIMEOUT);
	else
		return recvFindAndFilter(F("OK"), F("\r\r\n"), F("\r\n\r\nOK"), list, COMMAND_TIMEOUT);
}

bool ESP::sATCIPMUX(uint8_t mode)
{
	String data;
	flush();
	this->serial->print(F("AT+CIPMUX="));
	this->serial->println(mode);

	data = recvString(F("OK"), F("Link is builded"), COMMAND_TIMEOUT);
	if (data.indexOf("OK") != -1) {
		return true;
	}
	return false;
}

bool ESP::sATCIPSERVER(uint8_t mode, uint32_t port)
{
	String data;
	if (mode) {
		flush();
		this->serial->print(F("AT+CIPSERVER=1,"));
		this->serial->println(port);

		data = recvString(F("OK"), F("no change"), COMMAND_TIMEOUT);
		if (data.indexOf("OK") != -1 || data.indexOf("no change") != -1) {
			return true;
		}
		return false;
	}
	else {
		flush();
		this->serial->println(F("AT+CIPSERVER=0"));
		return recvFind(F("\r\r\n"), COMMAND_TIMEOUT);
	}
}

bool ESP::sATCIPSTO(uint32_t timeout)
{
	flush();
	this->serial->print(F("AT+CIPSTO="));
	this->serial->println(timeout);
	return recvFind(F("OK"), COMMAND_TIMEOUT);
}

bool ESP::eATCIPSTATUS(String &list)
{
	String data;
	delay(100);
	flush();
	this->serial->println(F("AT+CIPSTATUS"));
	return recvFindAndFilter(F("OK"), F("\r\r\n"), F("\r\n\r\nOK"), list, COMMAND_TIMEOUT);
}

bool ESP::sATCIPSENDMultiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len)
{
	flush();
	this->serial->print(F("AT+CIPSEND="));
	this->serial->print(mux_id);
	//this->serial->print('0');
	this->serial->print(F(","));
	this->serial->println(len);
	if (recvFind(F(">"), FIND_TIMEOUT)) {
		flush();
		for (uint32_t i = 0; i < len; i++) {
			this->serial->write(buffer[i]);
		}
		return recvFind(F("SEND OK"), FIND_OK_TIMEOUT);
	}
	return false;
}

bool ESP::sATCIPCLOSEMulitple(uint8_t mux_id)
{
	String data;
	flush();
	this->serial->print(F("AT+CIPCLOSE="));
	this->serial->println(mux_id);

	data = recvString(F("OK"), F("link is not"), 5000);
	if (data.indexOf(F("OK")) != -1 || data.indexOf(F("link is not")) != -1) {
		return true;
	}
	return false;
}