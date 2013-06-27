#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "ansi.h"

void fgcolor(unsigned char foreground) {
/*  Value      foreground     Value     foreground
    ------------------------------------------------
      0        Black            8       Dark Gray
      1        Red              9       Light Red
      2        Green           10       Light Green
      3        Brown           11       Yellow
      4        Blue            12       Light Blue
      5        Purple          13       Light Purple
      6        Cyan            14       Light Cyan
      7        Light Gray      15       White
*/
  int type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%dm", ESC, type, foreground+30);
}

void bgcolor(unsigned char background) {
/* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
   Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
                The designers of HyperTerminal, however, preferred black text on white background, which is why
                the colors are initially like that, but when the background color is first changed there is no
 	              way comming back.
   Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

    Value      Color      
    ------------------
      0        Black
      1        Red
      2        Green
      3        Brown
      4        Blue
      5        Purple
      6        Cyan
      7        Gray
*/
  printf("%c[%dm", ESC, background+40);
}

void color(unsigned char foreground, unsigned char background) { // Combination of fgcolor() and bgcolor() - uses less bandwidth
  int type = 22; // Normal text
	if (foreground > 7) {
	  type = 1; // Bold text
		foreground -= 8;
	}
  printf("%c[%d;%d;%dm", ESC, type, foreground+30, background+40);
}

void resetbgcolor() { // Reset background color
  printf("%c[m", ESC); // Gray on black text, no underline, no blink, no reverse
}

void clrscr() { // Clear screen
	printf("%c[2J", ESC);
}

void clreol() { // Clear line
	printf("%c[K", ESC);	
}

void gotoxy(unsigned char x, unsigned char y) { // Move the marker to the (x,y)-coordinate
	printf("%c[%d;%dH", ESC, y, x);
}

void graphicCommand(char command) { // Private helper function
	printf("%c[%dm", ESC, command);
}

void underline(char on) { // Used to turn underline on and off
	char command = 4;
	if (!on)
		command += 20;
	graphicCommand(command);
}

void blink(char on) { // Used to blink the text
	char command = 5;
	if (!on)
		command += 20;
	graphicCommand(command);	
}

void reverse(char on) { // Used to reverse the background and forground of the text
	char command = 7;
	if (!on)
		command += 20;
	graphicCommand(command);
}

unsigned char strlen(char* string) { // Calculate the length of a string
	unsigned char length = 0;
	while(*string++ != '\0')
		length++;
	return length;
}

// Used to draw a horizontal line with the corners specified in left and right
void drawTopBot(unsigned char x, unsigned char y, unsigned char width, unsigned char left, unsigned char right, unsigned char side) {
	int i;
	gotoxy(x,y);
	printf("%c",left);
	for (i=0;i<width;i++)
		printf("%c",side);
	printf("%c",right);
}

// Used to draw vertical sides
void drawSides(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char side) {
	int i, j;
	for (i=y1+1;i<y2;i++) {
		gotoxy(x1,i);
		printf("%c",side);
		gotoxy(x2,i);
		printf("%c",side);
	}
}

void saveCursor() { // Save the position of the cursor
	printf("%c[s", ESC);
}

void getSavedCursor() { // Get the last saved position
	printf("%c[u", ESC);
}

void moveCursor(char dir, unsigned char n) { // Move cursor in some direction
	printf("%c[%d%c", ESC, n, dir);
}