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

void color(unsigned char foreground, unsigned char background) { // combination of fgcolor() and bgcolor() - uses less bandwidth
  int type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%d;%dm", ESC, type, foreground+30, background+40);
}

void resetbgcolor() {
  printf("%c[m", ESC); // Gray on black text, no underline, no blink, no reverse
}

void clrscr() {
	printf("%c[2J", ESC);	
}

void clreol() {
	printf("%c[K", ESC);	
}

void gotoxy(unsigned char x, unsigned char y) {
	printf("%c[%d;%dH", ESC, y, x);
}

void graphicCommand(char command) {
	printf("%c[%dm", ESC, command);
}

void underline(char on) {
	char command = 4;
	if (!on)
		command += 20;
	graphicCommand(command);
}

void blink(char on) {
	char command = 5;
	if (!on)
		command += 20;
	graphicCommand(command);	
}

void reverse(char on) {
	char command = 7;
	if (!on)
		command += 20;
	graphicCommand(command);
}

unsigned char strlen(char* string) {
	unsigned char length = 0;
	while(*string++ != '\0')
		length++;
	return length;
}

void drawTopBot(unsigned char x, unsigned char y, unsigned char width, unsigned char left, unsigned char right, unsigned char side) {
	int i;
	gotoxy(x,y);
	printf("%c",left);
	for (i=0;i<width;i++)
		printf("%c",side);
	printf("%c",right);
}

void drawSides(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char side) {
	int i, j;
	for (i=y1+1;i<y2;i++) {
		gotoxy(x1,i);
		printf("%c",side);
		gotoxy(x2,i);
		printf("%c",side);
		//for (j=x1+1;j<x2;j++)
			//printf(" ");
		//printf("%c",side);
	}
}

void drawBanner(unsigned char x1, unsigned char y1, unsigned char left, unsigned char right, char* title) {
	gotoxy(x1+1,y1);
	printf("%c",left);
	reverse(1);
	printf("  %s  ",title);
	reverse(0);
	printf("%c",right);
}

void saveCursor() {
	printf("%c[s", ESC);
}

void getSavedCursor() {
	printf("%c[u", ESC);
}

void window(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, char* title, char style) {
	char* bannerTitle = title;
	unsigned char leftTop, rightTop, leftBot, rightBot, verSide, horSide, leftCross, rightCross;

	if (style) {
		leftTop = 201;
		rightTop = 187;
		leftBot = 200;
		rightBot = 188;
		verSide = 186;
		horSide = 205;
		leftCross = 185;
		rightCross = 204;
	} else {
		leftTop = 218;
		rightTop = 191;
		leftBot = 192;
		rightBot = 217;
		verSide = 179;
		horSide = 196;
		leftCross = 180;
		rightCross = 195;
	}
	
	drawTopBot(x1,y1,x2-x1-1,leftTop,rightTop,horSide);
	drawSides(x1,y1,x2,y2,verSide);
	drawTopBot(x1,y2,x2-x1-1,leftBot,rightBot,horSide);
	
	if (strlen(title) >  x2-x1-7)
		bannerTitle = "Err";
	drawBanner(x1,y1,leftCross,rightCross,bannerTitle);

	gotoxy(x1+2,y1+2);

	saveCursor();
}

void moveCursor(char dir, unsigned char n) {
	printf("%c[%d%c", ESC, n, dir);
}

void printWindow(char* string) {
	int position = 0;
	getSavedCursor();
	while(*string != '\0') {
		if (*string == '\n') {
			moveCursor(DOWN,1);
			moveCursor(BACK,position);
			position = 0;
		} else {
			printf("%c",*string);
			position++;
		}
		string++;
	}
	saveCursor();
}