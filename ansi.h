#ifndef _ansi_h_
#define _ansi_h_

#define ESC 0x1B

#define UP		'A'
#define DOWN	'B'
#define FORWARD	'C'
#define BACK	'D'

void fgcolor(int foreground);
void bgcolor(int background);
void color(int foreground, int background);
void resetbgcolor();
void clrscr();
void clreol();
void gotoxy(int x, int y);
void graphicCommand(char command);
void underline(char on);
void blink(char on);
void reverse(char on);
unsigned char strlen(char* string);
void drawTopBot(int x1, int x2, unsigned char left, unsigned char right, unsigned char side);
void drawSides(int x1, int y1, int x2, int y2, unsigned char side);
void drawBanner(int x1, int y1, unsigned char left, unsigned char right, char* title);
void saveCursor();
void getSavedCursor();
void window(int x1, int y1, int x2, int y2, char* title, char style);
void moveCursor(char dir, unsigned char n);
void printWindow(char* string);

#endif
