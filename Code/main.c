#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "reflexball.h"
#include "ansi.h"
#include "LED.h"
#include "time.h"
#include "buttons.h"
#include "gameport.h"
#include "asciidisplay.h"
#include "ascii.h"

unsigned long wheelTimer; // Used to add a small delay between reading the analog wheel

void main() {
	int input; // Input from the keyboard
	unsigned char buttons; // Input from either the hardware buttons or the sterring wheel's button
	char wheel; // Output from readSterringWheel()

	initTimers(); // Initialize the Timer1 used for delay_ms() and mills()
	initLED(); // Initialize LED display
	initButtons(); // Initialize hardware buttons
	initGameport(); // Initialize steering wheel buttons and driving wheel
	init_uart(_UART0,_DEFFREQ,BAUD_115200); // Start the UART at 115200 8N1

	color(2,0); // Green forground, black background

Start:
	clrscr(); // Clear the screen

	LEDsetString("    ReflexBall RALLY!"); // Scroll this text on the display

	initStartMenu(3,1,224,82); // x1, y1, x2, y2
	while(!startMenu()); // Wait for any key to be pressed
	clrscr();
	printMenu();
	while(!updateMenu()); // Wait until difficulty is choosen

	LEDsetString("    "); // Clear display
	initReflexBall(3,15,224,82,1); // x1, y1, x2, y2, style

	for(;;) {
		buttons = getGameportButtons(); // Read steering wheel's button
		
		if (buttons & 0xE) // Gear backward or button press
			startGame();
		else if (millis() - wheelTimer > 20) { // We have to limit the update rate or it will move to fast
			wheelTimer = millis();
			wheel = readSteeringWheel(); // Read sterring wheel
			if (wheel != 0)
				moveStriker(wheel);
		}
		else { // The driving wheel overrules the other controls
			buttons = readButtons(); // Hardware buttons
			if (buttons) {
				if (buttons & 0x2) // Center
					startGame();
				else if (buttons & 0x4) // Left
					moveStriker(-1);
				else if (buttons & 0x1) // Right
					moveStriker(1);
			}
			else if (kbhit()) { // Check if a buttons has been pressed on the keyboard
				input = getch(); // Read the keyboard
				if (input == ' ') // Space
					startGame();
				else if (input == 68) // Left
					moveStriker(-2);
				else if (input == 67) // Right
					moveStriker(2);
			}
		}
		updateGame(); // Update the game - this has to be called frequenctly
		if (restartGame)
			goto Start; // Goto back to the start of the game
	}
}
