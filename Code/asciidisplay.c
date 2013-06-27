#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "asciidisplay.h"
#include "ansi.h"
#include "LED.h"
#include "time.h"
#include "buttons.h"
#include "gameport.h"
#include "ascii.h"
#include "reflexball.h"

const unsigned char ballMenuYPos[4] = { 40, 48, 55, 64 };
const unsigned char ballXPos = 85;
unsigned char ballY, lastBallY;

unsigned char menuState;
unsigned long timer;
unsigned char oldButtonsWheel, oldButtonsBoard;

unsigned char xMin, yMin, xMax, yMax;

const char *startString = "Press any key to continue";

unsigned char strlen_rom(rom const char *string) { // Calculate the length of a string stored in the ROM
	unsigned char length = 0;
	while ((char)*string++ != '\0')
		length++;
	return length;
}

void printAscii(rom const char *str, unsigned char size) { // Used to print an ASCII art array at the last saved cursor position
	unsigned char i;

	for (i=1;i<=size;i++) {
		getSavedCursor();
		moveCursor(DOWN,i);
		while ((char)*str != '\0')
			printf("%c",*str++);
		str++; // Skip the null-terminater
	}
	getSavedCursor();
	moveCursor(DOWN,size);
	saveCursor();
}

void printAsciiXY(rom const char *str, unsigned char size, unsigned char x, unsigned char y) { // Used to print an ASCII art array at the specifid (x,y)-coordinate
	unsigned char i, length = strlen_rom(str);	
	for (i=0;i<size;i++) {
		gotoxy(x,y+i);
		while ((char)*str != '\0')
			printf("%c",*str++);
		str++; // Skip the null-terminater
	}
	moveCursor(BACK,length);
}

void clearMenuBall(unsigned char x, unsigned char y) { // Clear the ball in the menu
	gotoxy(x,y);
	printf("    ");
	gotoxy(x,y+1);
	printf("    ");
}

void drawMenuBall(unsigned char x, unsigned char y) { // Draw the ball in the menu
	const unsigned char top = 238, bottom = 95, slash = '/', backSlash = '\\';

	clearMenuBall(ballXPos,lastBallY);

	lastBallY = y;

	gotoxy(x,y);
	printf("%c%c%c%c",slash,top,top,backSlash);
	gotoxy(x,y+1);
	printf("%c%c%c%c",backSlash,bottom,bottom,slash);
}

void moveBall(char dir) { // Move the menu ball up and down	
	if (ballY + dir >= sizeof(ballMenuYPos) || ballY + dir < 0)
		return;

	ballY += dir;
	drawMenuBall(ballXPos,ballMenuYPos[ballY]);
}

void showWon() { // Shown when the user wins the game
	const char* highscoreString = "Good try! But the highscore is still held by:";
	clrscr();
	if (divider == 1) { // Chuck Norris mode		
		printAsciiXY(chuckNorrisAscii[0],sizeof(chuckNorrisAscii)/sizeof(chuckNorrisAscii[0]),(xMin+xMax)/2-strlen_rom(chuckNorrisAscii[0])/2,(yMin+yMax)/2-(sizeof(chuckNorrisAscii)/sizeof(chuckNorrisAscii[0]))/2-10);
		moveCursor(DOWN,1);
		moveCursor(BACK,strlen_rom(onlyChuckAscii[0])/2-strlen_rom(chuckNorrisAscii[0])/2);
		saveCursor();
		printAscii(onlyChuckAscii[0],sizeof(onlyChuckAscii)/sizeof(onlyChuckAscii[0]));
	} else {
		gotoxy(0,0);
		saveCursor();
		printAscii(ladyAscii[0],sizeof(ladyAscii)/sizeof(ladyAscii[0]));

		gotoxy((xMin+xMax)/2-strlen_rom(congratulationsAscii[0])/2+strlen_rom(ladyAscii[0])/2,20);		
		saveCursor();
		printAscii(congratulationsAscii[0],sizeof(congratulationsAscii)/sizeof(congratulationsAscii[0]));
		moveCursor(DOWN,1);
		moveCursor(FORWARD,strlen_rom(congratulationsAscii[0])/2-strlen_rom(nowTryAscii[0])/2);
		saveCursor();
		printAscii(nowTryAscii[0],sizeof(nowTryAscii)/sizeof(nowTryAscii[0]));
		
		moveCursor(DOWN,10);
		moveCursor(FORWARD,strlen_rom(nowTryAscii[0])/2-strlen(highscoreString)/2);
		blink(1);
		printf("%s",highscoreString);
		blink(0);
		moveCursor(DOWN,1);
		moveCursor(BACK,strlen(highscoreString)+strlen_rom(chuckNorrisTextAscii[0])/2-strlen(highscoreString)/2);
		saveCursor();
		printAscii(chuckNorrisTextAscii[0],sizeof(chuckNorrisTextAscii)/sizeof(chuckNorrisTextAscii[0]));
	}

	while (!getGameportButtons() && !readButtons() && !kbhit()); // Wait for button press
}

void showGameOver() { // Shown when the user lose
	clrscr();
	printAsciiXY(gameOverAscii[0],sizeof(gameOverAscii)/sizeof(gameOverAscii[0]),(xMin+xMax)/2-strlen_rom(gameOverAscii[0])/2,(yMin+yMax)/2-(sizeof(gameOverAscii)/sizeof(gameOverAscii[0]))/2-5);
	moveCursor(DOWN,1);
	
	switch (millis() & 0x7) { // Pseudo random number from 0-7
		case 0:
			if (strlen_rom(gameOverAscii[0]) > strlen_rom(amigoAscii[0]))
				moveCursor(FORWARD,strlen_rom(gameOverAscii[0])/2-strlen_rom(amigoAscii[0])/2);
			else
				moveCursor(BACK,strlen_rom(amigoAscii[0])/2-strlen_rom(gameOverAscii[0])/2);
			saveCursor();
			printAscii(amigoAscii[0],sizeof(amigoAscii)/sizeof(amigoAscii[0]));
			break;
		case 1:
			if (strlen_rom(gameOverAscii[0]) > strlen_rom(driveAscii[0]))
				moveCursor(FORWARD,strlen_rom(gameOverAscii[0])/2-strlen_rom(driveAscii[0])/2);	
			else
				moveCursor(BACK,strlen_rom(driveAscii[0])/2-strlen_rom(gameOverAscii[0])/2);
			saveCursor();
			printAscii(driveAscii[0],sizeof(driveAscii)/sizeof(driveAscii[0])); // Center text below "Game Over!"
			break;
		case 2:
			if (strlen_rom(gameOverAscii[0]) > strlen_rom(havNoBallsAscii[0]))
				moveCursor(FORWARD,strlen_rom(gameOverAscii[0])/2-strlen_rom(havNoBallsAscii[0])/2);	
			else
				moveCursor(BACK,strlen_rom(havNoBallsAscii[0])/2-strlen_rom(gameOverAscii[0])/2);
			saveCursor();
			printAscii(havNoBallsAscii[0],sizeof(havNoBallsAscii)/sizeof(havNoBallsAscii[0])); // Center text below "Game Over!"
			break;
		case 3:
			if (strlen_rom(gameOverAscii[0]) > strlen_rom(openEyesAscii[0]))
				moveCursor(FORWARD,strlen_rom(gameOverAscii[0])/2-strlen_rom(openEyesAscii[0])/2);
			else
				moveCursor(BACK,strlen_rom(openEyesAscii[0])/2-strlen_rom(gameOverAscii[0])/2);
			saveCursor();
			printAscii(openEyesAscii[0],sizeof(openEyesAscii)/sizeof(openEyesAscii[0])); // Center text below "Game Over!"
			break;
		case 4:
			if (strlen_rom(gameOverAscii[0]) > strlen_rom(patienceAscii[0]))
				moveCursor(FORWARD,strlen_rom(gameOverAscii[0])/2-strlen_rom(patienceAscii[0])/2);	
			else
				moveCursor(BACK,strlen_rom(patienceAscii[0])/2-strlen_rom(gameOverAscii[0])/2);
			saveCursor();
			printAscii(patienceAscii[0],sizeof(patienceAscii)/sizeof(patienceAscii[0])); // Center text below "Game Over!"
			break;
		case 5:
			if (strlen_rom(gameOverAscii[0]) > strlen_rom(notPassAscii[0]))
				moveCursor(FORWARD,strlen_rom(gameOverAscii[0])/2-strlen_rom(notPassAscii[0])/2);	
			else
				moveCursor(BACK,strlen_rom(notPassAscii[0])/2-strlen_rom(gameOverAscii[0])/2);
			saveCursor();
			printAscii(notPassAscii[0],sizeof(notPassAscii)/sizeof(notPassAscii[0])); // Center text below "Game Over!"
			break;
		case 6:
			if (strlen_rom(gameOverAscii[0]) > strlen_rom(thereIsNoBallAscii[0]))
				moveCursor(FORWARD,strlen_rom(gameOverAscii[0])/2-strlen_rom(thereIsNoBallAscii[0])/2);	
			else
				moveCursor(BACK,strlen_rom(thereIsNoBallAscii[0])/2-strlen_rom(gameOverAscii[0])/2);
			saveCursor();
			printAscii(thereIsNoBallAscii[0],sizeof(thereIsNoBallAscii)/sizeof(thereIsNoBallAscii[0])); // Center text below "Game Over!"
			break;
		case 7:
			if (strlen_rom(gameOverAscii[0]) > strlen_rom(deadAscii[0]))
				moveCursor(FORWARD,strlen_rom(gameOverAscii[0])/2-strlen_rom(deadAscii[0])/2);	
			else
				moveCursor(BACK,strlen_rom(deadAscii[0])/2-strlen_rom(gameOverAscii[0])/2);
			saveCursor();
			printAscii(deadAscii[0],sizeof(deadAscii)/sizeof(deadAscii[0])); // Center text below "Game Over!"
			break;
	}
	while (!getGameportButtons() && !readButtons() && !kbhit()); // Wait for button press
}

void initStartMenu(unsigned char newX1, unsigned char newY1, unsigned char newX2, unsigned char newY2) { // Start up the screen shown at startup
	xMin = newX1;
	yMin = newY1;
	xMax = newX2;
	yMax = newY2;
	
	blink(1);
	gotoxy((xMin+xMax)/2-strlen(startString)/2,(yMin+yMax)/2-10);
	printf("%s",startString);
	blink(0);	

	printAsciiXY(wheelAscii[0],sizeof(wheelAscii)/sizeof(wheelAscii[0]),(xMin+xMax)/2-strlen_rom(wheelAscii[0])/2,yMax-sizeof(wheelAscii)/sizeof(wheelAscii[0])-15);
	
	gotoxy((xMin+xMax)/2-strlen_rom(titleAscii1[0])/2,(yMin+yMax)/2-25);
	saveCursor();
	
	timer = 0;
}

unsigned char startMenu() { // Run the animation until a button is pressed
	if (millis() - timer > 200) {
		timer = millis();		

		switch (menuState) {
			case 0:
				printAscii(titleAscii1[0],sizeof(titleAscii1)/sizeof(titleAscii1[0]));
				moveCursor(UP,sizeof(titleAscii1)/sizeof(titleAscii1[0]));
				saveCursor();
				menuState = 1;
				break;
			 case 1:
				printAscii(titleAscii2[0],sizeof(titleAscii2)/sizeof(titleAscii2[0]));
				moveCursor(UP,sizeof(titleAscii2)/sizeof(titleAscii2[0]));
				saveCursor();
				menuState = 0;
				break;
		}
	}

	if (getGameportButtons()) {
 		oldButtonsWheel = getGameportButtons(); // Update oldButtons value, so it doesn't skip the next menu
 	    return 1;
 	} else if (readButtons()) {
		oldButtonsWheel = readButtons(); // Update oldButtons value, so it doesn't skip the next menu
 	    return 1;
	} else if (kbhit())
		return 1;

	return 0;
}

void printMenu() { // Print the difficulty menu
	printAsciiXY(menuAscii[0],sizeof(menuAscii)/sizeof(menuAscii[0]),(xMin+xMax)/2-strlen_rom(menuAscii[0])/2,(yMin+yMax)/2-15);

	moveCursor(FORWARD,strlen_rom(menuAscii[0])/2-strlen_rom(chuckAscii1[0])/2+10);
	moveCursor(DOWN,1);
	saveCursor();

	printAscii(easyAscii[0],sizeof(easyAscii)/sizeof(easyAscii[0]));
	printAscii(mediumAscii[0],sizeof(mediumAscii)/sizeof(mediumAscii[0]));
	moveCursor(DOWN,1);
	saveCursor();
	printAscii(hardAscii[0],sizeof(hardAscii)/sizeof(hardAscii[0]));
	moveCursor(DOWN,1);
	saveCursor();
	
	menuState = 0;
	timer = 0;

	ballY = lastBallY = 0;
	drawMenuBall(ballXPos,ballMenuYPos[ballY]);
}

unsigned char updateMenu() { // This is run until the user selects a difficulty
	int input;
	unsigned char buttons, buttonsClick;

	switch (menuState) {
		case 0:
			if (millis() - timer > 200) {
				timer = millis();
				printAscii(chuckAscii1[0],sizeof(chuckAscii1)/sizeof(chuckAscii1[0]));
				moveCursor(UP,sizeof(chuckAscii1)/sizeof(chuckAscii1[0]));
				saveCursor();
				menuState = 1;
			}
			break;
		 case 1:
			if (millis() - timer > 200) {
				timer = millis();
				printAscii(chuckAscii2[0],sizeof(chuckAscii2)/sizeof(chuckAscii2[0]));
				moveCursor(UP,sizeof(chuckAscii2)/sizeof(chuckAscii2[0]));
				saveCursor();
				menuState = 0;
			}
			break;
	}
	buttons = getGameportButtons();
	if (buttons != oldButtonsWheel) {
		buttonsClick = buttons & ~oldButtonsWheel; // Only look at the buttons that have changed
		oldButtonsWheel = buttons;

		if (buttonsClick & 0x1) // Gear forward
			moveBall(-1);
		else if (buttonsClick & 0x4) // Gear backward
			moveBall(1);
		else if (buttonsClick & 0xA) { // Either of the wheel buttons
			calculateDifficulty();
			return 1;
		}
	} else if (kbhit()) {
		input = getch();
		if (input == ' ') { // Space
			calculateDifficulty();
			return 1;
		} else if (input == 65) // Up
			moveBall(-1);
		else if (input == 66) // Down
			moveBall(1);
	} else {
		buttons = readButtons();
		if (buttons != oldButtonsBoard) {
			buttonsClick = buttons & ~oldButtonsBoard; // Only look at the buttons that have changed
			oldButtonsBoard = buttons;

			if (buttonsClick & 0x2) { // Center
				calculateDifficulty();
				return 1;
			} else if (buttonsClick & 0x4) // Left
				moveBall(-1);
			else if (buttonsClick & 0x1) // Right
				moveBall(1);
		}
	}
	return 0;
}

void calculateDifficulty() { // Calculate the difficulty based on the y-coordinates of the ball
	if (ballY == 0) { // Easy
		divider = 10;
		strikerWidth = 30;
	} else if (ballY == 1) { // Medium
		divider = 5;
		strikerWidth = 20;
	} else if (ballY == 2) { // Hard
		divider = 2;
		strikerWidth = 10;
	} else { // Chuck Norris
		divider = 1;
		strikerWidth = 4;
	}
}
