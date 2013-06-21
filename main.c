#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "reflexball.h"
#include "ansi.h"
#include "LED.h"
#include "time.h"
#include "gameport.h"

//char rom LEDtext[] = "Gruppe 15 er awesome! ��";
//char rom LEDtext[] = "�����ŵ��";
//char rom LEDtext[] = "Hej med dig!";
//char rom LEDtext[] = "    Gruppe 15 �";
char rom LEDtext[] = "�� �� ";

void main() {
	int input;
	unsigned char buttons;
	char wheel;

	initTimers();
	LEDinit();
	//LEDsetString(LEDtext);
	initButtons();
	initGameport();
	init_uart(_UART0,_DEFFREQ,BAUD_115200);
	color(2,4); // (2,0) for black background
	clrscr();
	initReflexBall(3,1,224,82,1); // x1, y1, x2, y2, style

	for(;;) {
		buttons = getGameportButtons();
		wheel = readSteeringWheel();
		gotoxy(10,20);
		printf("Buttons: %X",buttons);
		gotoxy(10,22);
		printf("Wheel: %02d",wheel);

		
		if (buttons & 0x4) // Gear forward
			startGame();
		else if (wheel != 0)
			moveStriker(wheel);
		else { // The driving wheel overrules the other controls
			buttons = readButtons();
			if (buttons) {
				if (buttons & 0x2) // Space
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
	}
} 
