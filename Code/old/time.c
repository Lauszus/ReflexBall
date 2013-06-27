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

	EI(); // Enable interrupt
}