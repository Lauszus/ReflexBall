#ifndef _time_h_
#define _time_h_

#define PRE1	(0 << 3);
#define PRE2	(1 << 3);
#define PRE4	(2 << 3);
#define PRE8	(3 << 3);
#define PRE16	(4 << 3);
#define PRE32	(5 << 3);
#define PRE64	(6 << 3);
#define PRE128	(7 << 3);

#define PRIORITY_TIMER0 (1 << 5)
#define PRIORITY_TIMER1 (1 << 6)
#define PRIORITY_TIMER2 (1 << 7)

// Public
void initTimers();
unsigned long millis();
void delay_ms(unsigned long time);

// Private
void timer1int();

#endif