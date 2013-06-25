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

typedef struct {
	unsigned long hour;
	unsigned char min, sec, cs;
} time_t;

extern volatile unsigned long delayTimer;
extern volatile unsigned char newTime;
extern volatile time_t time;

void resetTime();
void printTime(unsigned splittime);
void timer0int();
void timer1int();
void delay_ms(unsigned long time);
void initButtons();
unsigned char readButtons();
unsigned char readkey();
void initWindow();
void initTimers();
unsigned long millis();

#endif
