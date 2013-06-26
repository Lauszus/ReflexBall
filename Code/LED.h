#ifndef _led_h_
#define _led_h_

#define PRE1	(0 << 3);
#define PRE2	(1 << 3);
#define PRE4	(2 << 3);
#define PRE8	(3 << 3);
#define PRE16	(4 << 3);
#define PRE32	(5 << 3);
#define PRE64	(6 << 3);
#define PRE128	(7 << 3);

#define SCROLL_SPEED	5

// Public
void initLED(); // Initialize the LED display
void LEDsetString(char *string); // Used to scroll a string
void LEDRunOnce(char *firstString, char* secondString); // Used to sroll the first string once and then show the second string afterwards	

// Private
void clockLed(unsigned char digit);
unsigned char convertChar(char input);
void moveVideoBuffer();
void LEDupdate();
void timer2int();

#endif
