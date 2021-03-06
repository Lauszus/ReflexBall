#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "charset.h"
#include "LED.h"
#include "ansi.h"

unsigned char digit = 0, column = 0, delayCounter = 0, index = 0, stringLength = 0;
char videoBuffer[5][6];

volatile char runOnce, *pSecondString; // runOnce is used scroll a text only once, the pointer will point to the second string that is shown after the scroll is complete
char *pString; // Pointer to string in ram


void clockLed(unsigned char digit) { // Clock the specific digit
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

unsigned char convertChar(char input) { // Convert to some of our own characters
	unsigned char c;
	if (input == '�')
		c = '~' + 1;
	else if (input == '�')
		c = '~' + 2;
	else if (input == '�')
		c = '~' + 3;
	else if (input == '�')
		c = '~' + 4;
	else if (input == '�')
		c = '~' + 5;
	else if (input == '�')
		c = '~' + 6;
	else if (input == '�')
		c = '~' + 7;
	else if (input == '�') // Smiley
		c = '~' + 8;
	else if (input == '�') // <3
		c = '~' + 9;
	else
		c = input;

	return c;
}

void LEDsetString(char *string) { // Set string to show on the display
	unsigned char i, j;	

	DI(); // Disbable all interrupts

	runOnce = 0; // Reset flag
	stringLength = strlen(string); // Calculate length of string
	pString = string; // Set pointer to the start of the string

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
			videoBuffer[i][j] = character_data[' '-0x20][j]; // Fill out the rest of the string with spaces if the string is less than five characters
	}

	digit = column = delayCounter = index = 0; // Reset all values used for multiplexing

	EI(); // Enable all interrupts
}

void LEDRunOnce(char *firstString, char* secondString) { // Used to sroll the first string once and then show the second string afterwards
	LEDsetString(firstString);
	runOnce = 1;
	pSecondString = secondString; // We will save the location of the second string
}

void moveVideoBuffer() { // Increment to the next character in the string
	unsigned char i, j;

	for (i=0; i < 5; i++) {
		for (j=0;j<5;j++) {
			if (i < 4)
				videoBuffer[i][j] = videoBuffer[i+1][j]; // Shift all one to the left
			else
				videoBuffer[4][j] = character_data[convertChar(*pString)-0x20][j]; // Read the next character in the string
		}
	}
	pString++;
	if (*pString == '\0') { // Check if we have reached the end of the string
		if (runOnce) { // This wil actually abort when it loads the last character in the 5th digit, so you have to put a space in end of the sentence
			runOnce = 0;
			LEDsetString(pSecondString);
		} else
			pString -= stringLength; // Go all the way back to the end of the string
	}
}

void LEDupdate() { // This function is called inside the interrupt
	PGOUT = (PGOUT & (1 << 7)) | *(&videoBuffer[0][0] + digit*6 + column + index);
	PEOUT |= 0x1F; // Set all cathodes high
	PEOUT &= ~(1 << (4-column)); // Set one cathodes low decided by column
	
	clockLed(digit);
	if (++digit == 4) {
		digit = 0;
		if (++column == 5) {
			column = 0;
			if (++delayCounter == SCROLL_SPEED && stringLength > 4) { // We don't have to scroll the text if there is less than five characters
				delayCounter = 0;				
				if (++index > 5) {
					index = 0;
					moveVideoBuffer();
				}
			}
		}
	}
}

#pragma interrupt
void timer2int() { // Interrupt function
	LEDupdate();
}

void initLED() { // Initialize Timer2 used for multiplexing of the display
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