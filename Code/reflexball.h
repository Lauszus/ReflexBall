#ifndef _reflexball_h_
#define _reflexball_h_

#include "ansi.h"
#include "math.h"

#define STRIKER_MAX_WIDTH	30 // This has to be even
#define STRIKER_MAX_ANGLE	64 // 360 deg = 512
#define NLIVES				3
#define DEFAULT_DIFFICULTY	40
#define UART_MAX_SPEED		20 // If it gets below this value, we will only draw it every second time or if it hits an object, this is because the UART can not send the characters fast enough
#define MAX_DIFFICULTY		10 // If speed gets under this value the UART can no longer keep up
#define BALL_WIDTH			4 // The modulus of this number should be even
#define BALL_HEIGHT			2

#define BRICK_TABLE_WIDTH	14
#define BRICK_TABLE_HEIGHT	20


typedef struct {
	long x, y; // (x,y) is in the top left corner - these use the 18.14 format
	unsigned char width, height;
	TVector vector;
} Ball;

typedef struct {
	unsigned char x, y, width;
} Striker;

typedef struct {
	unsigned char x, y; // (x,y) is in the top left corner
	unsigned char lives;
	unsigned char width, height;
} Brick;

extern unsigned char divider; // This is the difficulty set in the beginning
extern unsigned char strikerWidth; // This is the striker width determent from the selected difficulty
extern unsigned char restartGame; // True if the user have won the game

// Public
void initReflexBall(unsigned char newX1, unsigned char newY1, unsigned char newX2, unsigned char newY2, char style);
void startGame();
void stopGame();
void updateGame();
void moveStriker(char dir);

// Private
void printLevel();
void printLives();
void printScore();
void showScoreLED();
void scrollLiveInGameLED();
void scrollLevelUp();
void scrollAll();
void dead();
unsigned char getTerminalCoordinate(long input);
void gotoxyBall(long x, long y);
void clearBigBall(long x, long y);
void drawBigBall();
void drawBrick(Brick *brick);
void checkIteration(unsigned char x, unsigned char y);
void setBallPos(unsigned char x, unsigned char y);
void iterate();
void drawStriker();
void ballPosStriker();
void initStriker(unsigned char x, unsigned y, unsigned char width);
void initBricks(char clear);
void initBall();
void drawLevel();
void levelUp();

#endif
