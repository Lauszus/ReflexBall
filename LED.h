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

#define LED_MAX_STR_LEN	50
#define SPACES			4
#define BUFFER_LENGTH	LED_MAX_STR_LEN+SPACES

void LEDinit();
void LEDsetString(char rom* string);
void LEDupdate();

#endif
