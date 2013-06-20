#ifndef _reflexball_h_
#define _reflexball_h_

#include "ansi.h"
#include "math.h"

#define STRIKER_MAX_WIDTH	30 // This has to be even
#define STRIKER_MAX_ANGLE	64 // 360 deg = 512
#define NLIVES				3
#define DEFAULT_DIFFICULTY	80 // 40
#define MAX_DIFFICULTY		20 // If speed gets under this value the UART can no longer keep up
#define BALL_WIDTH			4 // The modulus of this number should be even
#define BALL_HEIGHT			2
#define STEP_INTERVAL		2

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

void initReflexBall(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, char style);
void startGame();
void updateGame();
void stopGame();
void moveStriker(char dir);
void levelUp();
void drawLevel();

#endif
