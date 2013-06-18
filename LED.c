#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "charset.h"
#include "LED.h"

/*
	Fix videoBuffer, så det blot er en pointer til strengen
*/



unsigned char newData = 0, digit = 0, column = 0, index = 0, stringLength = 0, counter = 0, maxValue;
char videoBuffer[BUFFER_LENGTH][6];

void clockLed(unsigned char digit) {
	if (digit == 0) {
		PEOUT &= ~(1 << 7);
		PEOUT |= (1 << 7);
	}
	else if (digit == 1) {
		PGOUT |= (1 << 7);
		PGOUT &= ~(1 << 7);
	}
	else if (digit == 2) {
		PEOUT &= ~(1 << 5);
		PEOUT |= (1 << 5);
	}
	else if (digit == 3) {
		PEOUT &= ~(1 << 6);
		PEOUT |= (1 << 6);
	}
}

#pragma interrupt
void timer2int() {
	newData = 1;
}

void LEDinit() {
	unsigned char i;
	PEDD = 0; // All output
	PGDD = 0; // All output
	PEOUT = 0x1F; // Set clocks to low and cathodes to high
	PGOUT = 0; // Set all low

	for (i=0;i<4;i++) // Turn all off by default
		clockLed(i);

   	DI(); // Disable interrupt

	T2CTL = 0; // TEN - disable timer
	T2CTL |= PRE1; // PRES - Prescaler
	T2CTL |= (1 << 0); // TMODE - continuous mode

	T2H = 0;
	T2L = 1;
	
	T2RH = 9216 >> 8; // Interrupt every 500us
	T2RL = 9216 & 0xFF;

	SET_VECTOR(TIMER2, timer2int); // Enter the timer0int function at each interrupt
	
	// Set timer2 priority to low
	IRQ0ENH &= ~(1 << 7);
	IRQ0ENL |= (1 << 7);

	T2CTL |= (1 << 7); // TEN - enable timer

	EI(); // Enable interrupt
}

unsigned char strlen_rom(char rom* string) {
	unsigned char length = 0;
	while(*string++ != '\0')
		length++;
	return length;
}

void LEDsetString(char rom* string) {
	unsigned char i, j, c;

	stringLength = strlen_rom(string);
	if (stringLength > LED_MAX_STR_LEN)
		stringLength = LED_MAX_STR_LEN;

	for (i=0;i<SPACES;i++) { // Put spaces in fron of the text
		for (j=0;j<5;j++)
			videoBuffer[i][j] = character_data[' '-0x20][j];
		videoBuffer[i][5] = 0;
	}

	for (i=0;i<stringLength;i++) { // Write text to buffer
		for (j=0;j<5;j++) {
			if (*string == 'æ')
				c = '~' + 1;
			else if (*string == 'ø')
				c = '~' + 2;
			else if (*string == 'å')
				c = '~' + 3;
			else if (*string == 'Æ')
				c = '~' + 4;
			else if (*string == 'Ø')
				c = '~' + 5;
			else if (*string == 'Å')
				c = '~' + 6;
			else if (*string == 'µ')
				c = '~' + 7;
			else if (*string == '§') // Smiley
				c = '~' + 8;
			else if (*string == '£') // <3
				c = '~' + 9;
			else
				c = *string;
			videoBuffer[SPACES+i][j] = character_data[c-0x20][j];
		}
		videoBuffer[SPACES+i][5] = 0;
		string++;
	}
	maxValue = 3*6+5+stringLength*6; // Max digit count * number of columns + max column count + stringLength * number of columns
}
/*
void moveVideoBuffer() {
	char i, j, buf;
	buf = videoBuffer[0][0];
	for (i=0;i<stringLength+SPACES;i++) {
		for (j=0;j<5;j++)
			videoBuffer[i][j] = videoBuffer[i][j+1];
		if (i < stringLength+SPACES-1)
			videoBuffer[i][5] = videoBuffer[i+1][0];
		else
			videoBuffer[i][5] = buf;
	}
}
*/
void LEDupdate() {
	if (newData) {
		newData = 0;
		PGOUT = (PGOUT & 0x80) | *(&videoBuffer[0][0] + ((digit*6 + column + index)%maxValue)); // The modulus will take care of putting spaces behind the text
		//PGOUT = (PGOUT & 0x80) | *(&videoBuffer[0][0] + digit*6 + column + index);
		//PGOUT = (PGOUT & 0x80) | videoBuffer[digit][column];
		PEOUT |= 0x1F;
		PEOUT &= ~(1 << (4-column));

		clockLed(digit);
		
		if (++digit == 4) {
			digit = 0;
			if (++column == 6) {
				column = 0;
				if (++counter == 6) {
					counter = 0;
					//if (++index > stringLength*6)
					if (++index > (stringLength+SPACES)*6)
						index = 0;
					//moveVideoBuffer();					
				}
			}
		}
	}
}