#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "gameport.h"
#include "ansi.h"

unsigned int readADC(unsigned char channel) {
	unsigned char inHigh, inLow;
	unsigned int ADC_data;

	ADCCTL = 0x80 | 0x20 | (channel & 0x0F); // Enable ADC on the selected channel and use external voltage (3.3V) as VREF
	while (ADCCTL & 0x80); // Wait for conversion to be completed
	inHigh = ADCD_H; // ADC high byte
	inLow = ADCD_L; // ADC low low byte
	ADC_data = ((unsigned int)inHigh << 2)| inLow >> 6; // ADC output word

	return ADC_data;
}

void initGameport() {
	PBDD = (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2); // Use PB2, PB3, PB4 and PB5 as digial inputs
	PBAF = (1 << 1) | (1 << 0); // Use PB0 and PB1 for ADC conversion
}

unsigned char getGameportButtons() {
	return (((~PBIN) & 0x3C) >> 2);
}

char readSteeringWheel() {
	int val = readADC(1);

	//gotoxy(50,10);
	//printf("Val: %04d",val);

	// The driving wheel is not linear therefor this table is needed
	if (val > 1000)
		return -6; // We will move it more aggressively to the side
	else if (val > 796)
		return -4;
	else if (val > 712)
		return -2;
	else if (val > 610)
		return -1;
	else if (val > 570)
		return 0;
	else if (val > 500)
		return 1;
	else if (val > 466)
		return 2;
	else if (val > 440)
		return 4;
	else
		return 6;
}
