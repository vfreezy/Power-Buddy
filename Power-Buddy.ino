/*								Power-Buddy.ino
!*******************************************************************************
!*
!*
!*
!*
!*
!*	PIN Assignments:
!*		MOSI		-	11				|\
!*		MISO		-	12				| \  SD Card
!*		CLK			-	13				| /
!*		SS			-	4				|/
!*		Sensor1		-	1 (Analog)		> Current Sensor 1
!*		Sensor2		-	2 (Analog)		> Current Sensor 2
!*		Sensor3		-	3 (Analog)		> Voltage Sensor
!*		ESP8266-RX	-	2				|\
!*		ESP8266-TX	-	3				|/	WiFi
!*
!*
!*
!*
!*****************************************************************************/
/*                              INCLUDES
******************************************************************************/
#include <SPI.h>
#include <SD.h>
#include "Current.h"
#include "Voltage.h"
#include "SecureDigitalStorage.h"


SecureDigitalStorage files((uint8_t) 4, (uint8_t *)"test7.txt");


void setup()
{
	Serial.begin(9600);
	delay(5000);

}

void loop()
{
	delay(5000);
	Serial.println("Loop");
}