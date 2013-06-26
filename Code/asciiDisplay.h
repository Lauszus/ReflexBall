#ifndef _startscreen_h_
#define _startscreen_h_

extern unsigned char x1, y1, x2, y2;

unsigned char strlen_rom(rom const char *string);
void printAscii(rom const char *str, unsigned char size);
void printAsciiXY(rom const char *str, unsigned char size, unsigned char x, unsigned char y);
void initStartMenu(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
unsigned char startMenu();
void printMenu();
unsigned char updateMenu();
void showWon();
void showGameOver();

#endif
