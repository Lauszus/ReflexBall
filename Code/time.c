#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "time.h"
#include "ansi.h"

volatile unsigned long delayTimer = 0, mscounter = 0;
volatile unsigned char newTime = 0;
volatile time_t time;

void resetTime() {
	time.hour = 0;
	time.min = 0;
	time.sec = 0;
	time.cs = 0;
	T0H = 0; // Reset timer value
	T0L = 1;
}

void printTime(unsigned splittime) {
	if (splittime != 3)
		gotoxy(32,7+splittime);
	else
		gotoxy(32,7);
	printf("%01d:%02d:%02d.",time.hour,time.min,time.sec);
	if (splittime)
		printf("%02d",time.cs);
	else
		printf("--");
}

#pragma interrupt
void timer0int() {
	time.cs++;

	if (time.cs == 100) {
		time.cs = 0;
		time.sec++;
		newTime = 1;
		if (time.sec == 60) {
			time.sec = 0;
			time.min++;
			if (time.min == 60) {
				time.min = 0;
				time.hour++; // To the infinity!!
			}
		}
	}
}

unsigned long millis() {
	return mscounter;
}

#pragma interrupt
void timer1int() {
	delayTimer--;
	mscounter++;
}

void delay_ms(unsigned long time) { // This not that acurate, but good enough for our needs
	delayTimer = time;
	while(delayTimer);
}

void initButtons() {
	PDDD = (1 << 3);
	PFDD = (1 << 6) | (1 << 7);
}

unsigned char readButtons() {
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

void initWindow() {
	color(2,4);
	clrscr();
	window(10,5,45,11,"Stop watch",1);
	printWindow("Time since start:   0:00:00.--\n");
	printWindow("Split time 1:       -:--:--.--\n");
	printWindow("Split time 2:       -:--:--.--\n");
}

void initTimers() {
	DI(); // Disable interrupt
/*
	T0CTL = 0; // TEN - disable timer
	T0CTL |= PRE4; // PRES - Prescaler
	T0CTL |= (1 << 0); // TMODE - continuous mode

	T0H = 0;
	T0L = 1;
	
	T0RH = 46080 >> 8; // Interrupt every 10ms
	T0RL = 46080 & 0xFF;

	SET_VECTOR(TIMER0, timer0int); // Enter the timer0int function at each interrupt
	
	// Set timer0 priority to high
	IRQ0ENH |= PRIORITY_TIMER0;
	IRQ0ENL |= PRIORITY_TIMER0;
	
	resetTime();

	//T0CTL |= (1 << 7); // TEN - enable timer
*/
	T1CTL = 0; // TEN - disable timer
	T1CTL |= PRE1; // PRES - Prescaler
	T1CTL |= (1 << 0); // TMODE - continuous mode

	T1H = 0;
	T1L = 1;
	
	T1RH = 18432 >> 8; // Interrupt every 1ms
	T1RL = 18432 & 0xFF;

	SET_VECTOR(TIMER1, timer1int); // Enter the timer1int function at each interrupt
	
	// Set timer1 priority to high
	IRQ0ENH |= PRIORITY_TIMER1;
	IRQ0ENL |= PRIORITY_TIMER1;
	
	T1CTL |= (1 << 7); // TEN - enable timer

	EI(); // Enable interrupt
}
