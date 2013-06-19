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
	initTimers();
	LEDinit();
	//LEDsetString(LEDtext);
	init_uart(_UART0,_DEFFREQ,BAUD_115200);
	color(2,4);
	clrscr();
	initReflexBall(3,1/*60*/,224,82,1); // x1, y1, x2, y2, style

	for(;;) {
		if (kbhit()) {
			input = getch();
			if (input == ' ') // Space
				startGame();
			else if (input == 68) // Left
				moveStriker(-1);
			else if (input == 67) // Right
				moveStriker(1);
		}
		updateGame();
		//LEDupdate();
	}
} 
