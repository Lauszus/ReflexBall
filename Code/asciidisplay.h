#ifndef _asciidisplay_h_
#define _asciidisplay_h_

// Public
unsigned char strlen_rom(rom const char *string);
void printAscii(rom const char *str, unsigned char size);
void printAsciiXY(rom const char *str, unsigned char size, unsigned char x, unsigned char y);
void initStartMenu(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
unsigned char startMenu();
void printMenu();
unsigned char updateMenu();
void showWon();
void showGameOver();

// Private
void clearMenuBall(unsigned char x, unsigned char y);
void drawMenuBall(unsigned char x, unsigned char y);
void moveBall(char dir);
void calculateDifficulty();

#endif
