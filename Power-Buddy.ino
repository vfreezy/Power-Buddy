/*								Power-Buddy.ino
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
#include <SPI.h>
#include <SD.h>
#include "Current.h"
#include "Voltage.h"
#include "SecureDigitalStorage.h"




void testFunctionSensors();
void testFunctionSD();
void setup()
{
	Serial.begin(9600);
	testFunctionSD();

}

void loop()
{
	delay(5000);
	Serial.println("Loop");

}
void testFunctionSD()
{
	SecureDigitalStorage files((uint8_t) 4, (uint8_t *)"test2.txt");
	files.writeln((uint8_t*)"line1",(uint8_t) 5);
	files.writeln((uint8_t*)"line2",(uint8_t) 5);
	files.writeln((uint8_t*)"line3",(uint8_t) 5);
	files.writeln((uint8_t*)"line4",(uint8_t) 5);
	uint8_t * temp = files.readln();
	
	if (temp != NULL)
		Serial.println((char *)temp);
}
void testFunctionSensors()
{
	Current cSensor(1);
	Voltage vSensor(2);
	
	cSensor.read();
	vSensor.read();
	
	Serial.print("Current = ");
	Serial.print(cSensor.getValue(),DEC);
	Serial.print(" | Voltage = ");
	Serial.println(vSensor.getValue(), DEC);
}