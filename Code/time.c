#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "time.h"
#include "ansi.h"

volatile unsigned long delayTimer, mscounter;

void initTimers() { // Initialize Timer1 to interrupt every 1ms
	DI(); // Disable interrupt

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
	
	delayTimer = 0;
	mscounter = 0;

	T1CTL |= (1 << 7); // TEN - enable timer

	EI(); // Enable interrupt
}

unsigned long millis() { // Get the time since program start in ms
	return mscounter;
}

void delay_ms(unsigned long time) { // Delay in ms - this is not that acurate, but good enough for our needs
	delayTimer = time;
	while(delayTimer);
}

#pragma interrupt
void timer1int() { // Interrupt function
	delayTimer--;
	mscounter++;
}
