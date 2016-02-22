/*								Power-Buddy.ino
!*******************************************************************************
!*	Description: Brains behind the Power Buddy!
!*
!*  Known Bugs:  Not finished yet
!*
!*
!*
!*
!*
!*
!*							PIN Assignments:
!*		=================================================================
!*		MOSI		-	11				|\
!*		MISO		-	12				| \  SD Card
!*		CLK			-	13				| /
!*		SS			-	10				|/
!*		Sensor1		-	1 (Analog)		> Current Sensor 1
!*		Sensor2		-	2 (Analog)		> Current Sensor 2
!*		Sensor3		-	3 (Analog)		> Voltage Sensor
!*		ESP8266-0	-	3				|\
!*		ESP8266-2	-	2				| \  WiFi	
!*		ESP8266-RX	-	1				| /
!*		ESP8266-TX	-	0				|/	
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
#include <SPI.h>
#include <SD.h>
#include "stdint.h"
#include "Current.h"
#include "Voltage.h"
#include "SecureDigitalStorage.h"
#include "CommLink.h"
#include "HardwareSerial.h"


/*                           Global Objects
******************************************************************************/
SecureDigitalStorage * file_Config;
SecureDigitalStorage * file_Log;
SecureDigitalStorage * file_Outlet1;
SecureDigitalStorage * file_Outlet2;

Current * sensorCurrent_1;
Current * sensorCurrent_2;
Voltage * sensorVoltage;

CommLink * wifi;
/*                         Global Variables
******************************************************************************/


/*                            Prototypes
******************************************************************************/
boolean setupSD();
boolean setupSensor();
boolean setupCommLink();
void serviceTCPServer();

void setup()
{
	#ifdef VERBOSE
		Serial.begin(BAUD_RATE);
		Serial.println(F("Beginning Setup"));
		Serial.println(F("SD INIT..."));
	#endif
	if (!setupSD()) {
		//SD failed to init, something should be done here
		#ifdef VERBOSE
			Serial.println(F("FAILED"));
		#endif
	}
	#ifdef VERBOSE
		Serial.println(F("WIFI INIT..."));
	#endif
	if (!setupCommLink()) {
		//WiFi failed to init, something should be done here
		#ifdef VERBOSE
			Serial.println(F("FAILED"));
		#endif
	}
	#ifdef VERBOSE
		Serial.println(F("Setup complete"));
	#endif
}


void loop()
{
	serviceTCPServer();
}


boolean setupSD() {
	file_Config = new SecureDigitalStorage((uint8_t)PIN_SD_SS, (uint8_t *)"Config.txt");
	file_Log = new SecureDigitalStorage((uint8_t)PIN_SD_SS, (uint8_t *)"Log.txt");
	file_Outlet1 = new SecureDigitalStorage((uint8_t)PIN_SD_SS, (uint8_t *)"O1.txt");
	file_Outlet2 = new SecureDigitalStorage((uint8_t)PIN_SD_SS, (uint8_t *)"O2.txt");

	return file_Outlet1->enabled();
}
boolean setupSensor() {
	//Do something with these
	sensorCurrent_1 = new Current(PIN_CURRENT_1);
	sensorCurrent_2 = new Current(PIN_CURRENT_2);
	sensorVoltage = new Voltage(PIN_VOLTAGE_1);
}
boolean setupCommLink() {
	//Create WiFi object
	wifi = new CommLink(Serial1);
	
	wifi->disconnect();
	//Check to see the ESP8266 is connected
	if (wifi->getStatus(STATUS_ESP)) {
		if (wifi->connect("Chance-2G", "disneyqueen")){
			if (wifi->startServer())
				return true;			
		}
	}
	//An error occured at some point
	return false;
}

void serviceTCPServer() {
	//Check for connected client
	if (wifi->haveClient()) {
		//What does the client want????
		uint8_t temp = wifi->clientWants();

		//Handle the client like a BOSS
		switch (temp){
			case MSG_DL_1:
				wifi->sendFile(file_Outlet1);
				break;
			case MSG_DL_2:
				wifi->sendFile(file_Outlet2);
				break;
			case MSG_DL_CFG:
				Serial.println("Detected");
				wifi->sendFile(file_Config);
				break;
			case MSG_DL_LOG:
				wifi->sendFile(file_Log);
				break;
			case MSG_UL_1:
				wifi->rcvFile(file_Outlet1);
				break;
			case MSG_UL_2:
				wifi->rcvFile(file_Outlet2);
				break;
			case MSG_UL_CFG:
				wifi->rcvFile(file_Config);
				break;
			case MSG_UL_LOG:
				wifi->rcvFile(file_Log);
				break;
			case MSG_RST:
				//Insert code to reset
				break;
			default:
				break;
		}

		//Client has been serviced, remove them from your presence
		if (!wifi->kickClient())
			Serial.println("Unable to close client");
		else
			Serial.println("Closed client");
	}
}