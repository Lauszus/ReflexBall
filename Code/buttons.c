#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "buttons.h"
#include "time.h"

void initButtons() { // Initialize the buttons on the evaluation board
	PDDD = (1 << 3);
	PFDD = (1 << 6) | (1 << 7);
}

unsigned char readButtons() { // Read the buttons buttons in the following order: bit2: PD3, bit1: PF6 and bit0: PF7
	unsigned char inD, inF;
	inD = (~PDIN >> 3) & 0x1;
	inF = (~PFIN >> 6) & 0x3;
	return ((inF >> 1) | ((inF & 0x1) << 1) | (inD << 2));
}

unsigned char readkey() { // Read button with debounce
	unsigned char output = readButtons();
	delay_ms(50); // Wait 50ms
	output &= readButtons(); // Check if it is still high
	return output;
}