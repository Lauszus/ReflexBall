#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "reflexball.h"
#include "ansi.h"
#include "LED.h"
#include "time.h"

//char rom LEDtext[] = "Gruppe 15 er awesome! ß£";
//char rom LEDtext[] = "Ê¯Â∆ÿ≈µß£";
//char rom LEDtext[] = "Hej med dig!";
//char rom LEDtext[] = "    Gruppe 15 £";
char rom LEDtext[] = "££ ££ ";

void main() {
	int input;
	unsigned char buttons;

	initTimers();
	LEDinit();
	//LEDsetString(LEDtext);
	initButtons();
	init_uart(_UART0,_DEFFREQ,BAUD_115200);
	color(2,4);
	clrscr();
	initReflexBall(3,1,224,82,1); // x1, y1, x2, y2, style

	for(;;) {
		buttons = readButtons();
		if (buttons) {
			if (buttons & 0x2) // Space
				startGame();
			else if (buttons & 0x4) // Left
				moveStriker(-1,1);
			else if (buttons & 0x1) // Right
				moveStriker(1,1);
		}
		else if (kbhit() || buttons) {
			input = getch();
			if (input == ' ' || buttons & 0x2) // Space
				startGame();
			else if (input == 68 || buttons & 0x4) // Left
				moveStriker(-1,2);
			else if (input == 67 || buttons & 0x1) // Right
				moveStriker(1,2);
		}
		updateGame();
	}
} 
