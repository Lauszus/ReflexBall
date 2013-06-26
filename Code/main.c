#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "reflexball.h"
#include "ansi.h"
#include "LED.h"
#include "time.h"
#include "gameport.h"
#include "startScreen.h"
#include "ascii.h"

void main() {
	int input;
	unsigned char buttons;
	char wheel;

	initTimers();
	initLED();
	initButtons();
	initGameport();
	init_uart(_UART0,_DEFFREQ,BAUD_115200);

	color(2,0); // Green forground, black background

Start:
	clrscr();

	LEDsetString("    ReflexBall RALLY!");

	initStartMenu(3,1,224,82); // x1, y1, x2, y2
	while(!startMenu()); // Wait for any key to be pressed
	clrscr();
	printMenu();
	while(!updateMenu()); // Wait until difficulty is choosen

	LEDsetString("    "); // Clear display
	initReflexBall(3,15,224,82,1); // x1, y1, x2, y2, style	

	for(;;) {
		buttons = getGameportButtons();
		wheel = readSteeringWheel();
		
		if (buttons & 0xE) // Gear backward or button press
			startGame();
		else if (wheel != 0)
			moveStriker(wheel);
		else { // The driving wheel overrules the other controls
			buttons = readButtons();
			if (buttons) {
				if (buttons & 0x2) // Center
					startGame();
				else if (buttons & 0x4) // Left
					moveStriker(-1);
				else if (buttons & 0x1) // Right
					moveStriker(1);
			}
			else if (kbhit()) {
				input = getch();
				if (input == ' ') // Space
					startGame();
				else if (input == 68) // Left
					moveStriker(-2);
				else if (input == 67) // Right
					moveStriker(2);
			}
		}
		updateGame();
		if (restartGame)
			goto Start;
	}
}
