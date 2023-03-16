#include "StringUtilities.h"
#include "UART.h"
#include "Accelerometer.h"
#include "Gyro.h"

void DisplayHeader()
{
	SendString("    Accel   Gyro\r\n");
}

void DisplayIntegerValue(short value)
{
	// Convert the number to string
	char valueAsString[12];
	IntegerToString(value, valueAsString, BASE_10);

	// Right justify display the string
	short strlen = 0;
	while(valueAsString[strlen])
	{
		strlen++;
	}

	while(strlen < 7)
	{
		SendString(" ");
		++strlen;
	}

	SendString(valueAsString);
}

void DisplayAxisValue(char* label, short accel, short gyro)
{
	SendString(label);
	SendString(":");
	DisplayIntegerValue(accel); 
	DisplayIntegerValue(gyro); 
}

void DisplayAxisValues()
{
	short accelX, accelY, accelZ;
	GetAccelerometerValues(&accelX, &accelY, &accelZ);

	short gyroX, gyroY, gyroZ;
	GetGyroValues(&gyroX, &gyroY, &gyroZ);

	DisplayAxisValue("X", accelX, gyroX);
	SendString("\r\n");
	DisplayAxisValue("Y", accelY, gyroY);
	SendString("\r\n");
	DisplayAxisValue("Z", accelZ, gyroZ);

	SendString("\033[2A");  // Makes cursor got up to lines
	SendString("\r");       // Return cursor to the beginning of the line
}