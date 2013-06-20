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

//#define STORE_STRING_IN_ROM

void LEDinit();
//void LEDupdate();

#ifdef STORE_STRING_IN_ROM
void LEDsetString(char rom *string);
#else
void LEDsetString(char *string);
#endif

void LEDRunOnce(char *firstString, char* secondString);

#endif
