#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "charset.h"
#include "LED.h"
#include "ansi.h"

unsigned char digit = 0, column = 0, delayCounter = 0, index = 0, stringLength = 0;
//unsigned char newData = 0;
char videoBuffer[5][6];


volatile char runOnce, *pSecondString;

#ifdef STORE_STRING_IN_ROM
char rom *pString; // Pointer to string in rom
#else
char *pString; // Pointer to string in ram
#endif

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

unsigned char strlen_rom(char rom* string) {
	unsigned char length = 0;
	while(*string++ != '\0')
		length++;
	return length;
}

unsigned char convertChar(char input) { // Convert to some of our own characters
	unsigned char c;
	if (input == 'æ')
		c = '~' + 1;
	else if (input == 'ø')
		c = '~' + 2;
	else if (input == 'å')
		c = '~' + 3;
	else if (input == 'Æ')
		c = '~' + 4;
	else if (input == 'Ø')
		c = '~' + 5;
	else if (input == 'Å')
		c = '~' + 6;
	else if (input == 'µ')
		c = '~' + 7;
	else if (input == '§') // Smiley
		c = '~' + 8;
	else if (input == '£') // <3
		c = '~' + 9;
	else
		c = input;

	return c;
}

#ifdef STORE_STRING_IN_ROM
void LEDsetString(char rom *string) {
#else
void LEDsetString(char *string) {
#endif
	unsigned char i, j;	

	DI(); // Disbable all interrupts

	runOnce = 0; // Reset flag

#ifdef STORE_STRING_IN_ROM
	stringLength = strlen_rom(string);
#else
	stringLength = strlen(string);
#endif
	pString = string;

	for (i=0; i < 5; i++) {
		for (j=0;j<5;j++)			
			videoBuffer[i][j] = character_data[convertChar(*pString)-0x20][j];
		videoBuffer[i][5] = 0;

		pString++;
		if (*pString == '\0') {
			pString -= stringLength;
			break; // Break if we have reached the end of the string - this is due to the string being less than five characters wide
		}
	}
	for (i = stringLength; i < 4; i++) {
		for (j = 0; j < 5; j++)
			videoBuffer[i][j] = character_data[' '-0x20][j]; // Fill out the rest of the string with spaces if the string is less that four characters
	}

	digit = column = delayCounter = index = 0; // Reset all values used for multiplexing

	EI(); // Enable all interrupts
}

void LEDRunOnce(char *firstString, char* secondString) {	
	LEDsetString(firstString);
	runOnce = 1;
	pSecondString = secondString; // We will save the location of the second string
}

void moveVideoBuffer() {
	unsigned char i, j;

	for (i=0; i < 5; i++) {
		for (j=0;j<5;j++) {
			if (i < 4)
				videoBuffer[i][j] = videoBuffer[i+1][j];
			else
				videoBuffer[4][j] = character_data[convertChar(*pString)-0x20][j];
		}
	}
	pString++;
	if (*pString == '\0') {
		if (runOnce) { // This wil actually abort when it loads the last character in the 5th digit, so you have to put a space in end of the sentence
			runOnce = 0;
			LEDsetString(pSecondString);
		} else
			pString -= stringLength;
	}
}

void LEDupdate() { // This function is called inside the interrupt
	//if (newData) {
		//newData = 0;
		PGOUT = (PGOUT & 0x80) | *(&videoBuffer[0][0] + digit*6 + column + index);
		PEOUT |= 0x1F;
		PEOUT &= ~(1 << (4-column));

		clockLed(digit);
		
		if (++digit == 4) {
			digit = 0;
			if (++column == 6) {
				column = 0;				
				if (++delayCounter == 5 && stringLength > 4) { // We don't have to scroll the text if there is less than four characters
					delayCounter = 0;				
					if (++index > 5) {
						index = 0;
						moveVideoBuffer();
					}
				}
			}
		}
	//}
}

#pragma interrupt
void timer2int() {
	//newData = 1;
	LEDupdate();
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