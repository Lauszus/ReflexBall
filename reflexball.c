#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "reflexball.h"
#include "time.h"

Ball ball;
Striker striker;
Brick bricks[BRICK_TABLE_HEIGHT][BRICK_TABLE_WIDTH];
unsigned char brickTable[BRICK_TABLE_HEIGHT][BRICK_TABLE_WIDTH];

unsigned long gameTimer;
unsigned char gameStarted = 0, x1, y1, x2, y2, alive, lives;
unsigned int score;

unsigned strikerAngle[STRIKER_MAX_WIDTH/2-1+BALL_WIDTH/2-1];

/*
void printLives() {
	//const unsigned char heart = 3;
	gotoxy(x2+3,y1+1);
	printf("Lives:    %d", lives);
}
void printScore() {
	const unsigned char dollar = 36;
	gotoxy(x2+3,y1+2);
	printf("Score: %04d %c", score, dollar);
}
*/
void printTextCenter(char* string) {
	gotoxy((x2-x1)/2+x1-strlen(string)/2,(y2-y1)/2+y1);
	printf("%s",string);
}

void dead() {
	if(--lives == 0)
		printTextCenter("Game Over!");
	//printLives();
	alive = 0;
	stopGame();
}

unsigned char getTerminalCoordinate(long input) {
	unsigned char output = input >> FIX14_SHIFT;
	output += (input >> (FIX14_SHIFT-1)) & 0x1; // Round correctly
	return output;
}

void gotoxyBall(long x, long y) {
	gotoxy(getTerminalCoordinate(x),getTerminalCoordinate(y));
}

void drawBigBall() {
	unsigned char i, top = 238, bottom = 95, slash = '/', backSlash = '\\';

	gotoxyBall(ball.x,ball.y);
	printf("%c%c%c%c",slash,top,top,backSlash);
	gotoxyBall(ball.x,ball.y + (1 << FIX14_SHIFT));
	printf("%c%c%c%c",backSlash,bottom,bottom,slash);
}

void clearBigBall() {
	gotoxyBall(ball.x,ball.y);
	printf("    ");
	gotoxyBall(ball.x,ball.y + (1 << FIX14_SHIFT));
	printf("    ");
}

void printBallCoor() {
	gotoxy(10,10);
	printf("Ball data: ");
	printFix(ball.x,2);
	printf(" ");
	printFix(ball.y,2);
	printf(" ");
	printFix(ball.vector.x,2);
	printf(" ");
	printFix(ball.vector.y,2);
}

void drawBrick(Brick *brick) {
	unsigned char i, j, brickStyle;
	for (i=0;i<brick->height;i++) {
		gotoxy(brick->x,brick->y+i);
		for (j=0;j<brick->width;j++) {
			if (!brick->lives)
				brickStyle = ' ';
			else if (brick->lives == 1)
				brickStyle = 176;
			else if (brick->lives == 2)
				brickStyle = 177;
			else if (brick->lives == 3)
				brickStyle = 178;
			else // 4 lives
				brickStyle = 219;				
			printf("%c",brickStyle);			
		}
	}
}

unsigned long counterX = 0, counterY = 0;

void drawBallxy(unsigned char x, unsigned char y) {
	unsigned char i, j, decreasedLife = 0, dontDeflectX = 0, dontDeflectY = 0, deflectedX = 0, deflectedY = 0, cornerHit = 0;
	char distance; // Distance from center to ball position on striker
	int angle;
	//printBallCoor();
	//gotoxy(10,10);
	//printf("x: %d y: %d strikerX: %d strikerY: %d",x,y,striker.x,striker.y);
	if ((x > x1 && x+ball.width < x2 && y > y1 && y+ball.height-1 < y2-1) || !gameStarted) {
		for (i=0;i<BRICK_TABLE_HEIGHT;i++) {
			for (j=0;j<BRICK_TABLE_WIDTH;j++) {
				if (!bricks[i][j].lives) // Skip if lives == 0
					continue;
				dontDeflectX = 0;
				dontDeflectY = 0;
				decreasedLife = 0;				

				// Check if the ball hit a brick
				if (y <= bricks[i][j].y+bricks[i][j].height-1 && y+ball.height-1 >= bricks[i][j].y && x+ball.width-1 >= bricks[i][j].x && x <= bricks[i][j].x+bricks[i][j].width-1) {

					// TODO: Skal det hele ikke være <= og >= ? og en af brikkerne dør ikke altid i et corner
					
					// Check if the ball hit the top or the bottom of the brick
					if (y <= bricks[i][j].y+bricks[i][j].height-1 && y+ball.height-1 >= bricks[i][j].y) { // && x+ball.width > bricks[i][j].x && x < bricks[i][j].x+bricks[i][j].width
						bricks[i][j].lives--;
						decreasedLife = 1;
						drawBrick(&bricks[i][j]);
						
			  		    if (!deflectedY) {
							if (y+ball.height-1 == bricks[i][j].y) { // Top of brick
								if (x+ball.width-1 == bricks[i][j].x || x == bricks[i][j].x+bricks[i][j].width-1) { // Top left or right corner
									if (i > 0) { // Make sure that there can actually be a brick above it
										if (bricks[i-1][j].lives) // Check if brick above is alive
											dontDeflectY = 1; // Don't deflect it
									}
								}
							} else if (y == bricks[i][j].y+bricks[i][j].height-1) { // Bottom of brick
								if (x+ball.width-1 == bricks[i][j].x || x == bricks[i][j].x+bricks[i][j].width-1) { // Bottom left or right corner
									if (i+1 < BRICK_TABLE_HEIGHT) { // Make sure that there can actually be a brick underneath it
										if (bricks[i+1][j].lives) // Check if brick below is alive
											dontDeflectY = 1; // Don't deflect it
									}
								}
							} else if (y == bricks[i][j].y) {
								dontDeflectY = 1;
								cornerHit = 0;
							}
							
							if (!dontDeflectY && !cornerHit) {
								deflectedY = 1;
								ball.vector.y = -ball.vector.y;
								ball.y += 2*ball.vector.y;
							} else if (dontDeflectY) {
								gotoxy(10,4);
								printf("dontDeflectY: %d",counterY++);
							}
						}													
					}
					// Check if the ball hit one of the sides of the brick
					if (x+ball.width-1 == bricks[i][j].x || x == bricks[i][j].x+bricks[i][j].width-1) { // y < bricks[i][j].y+bricks[i][j].height && y+ball.height > bricks[i][j].y && x+ball.width >= bricks[i][j].x && x < bricks[i][j].x+bricks[i][j].width					
						if (!decreasedLife) { // Make sure it is has not already lost a life
							bricks[i][j].lives--;
							drawBrick(&bricks[i][j]);
						}

						if (!deflectedX) {
							if (x+ball.width-1 == bricks[i][j].x) { // Left side of brick
								if (y+ball.height-1 == bricks[i][j].y || y == bricks[i][j].y+bricks[i][j].height-1) { // Top or bottom left corner
									if (j > 0 && bricks[i][j-1].lives) {
										if (dontDeflectY)
											cornerHit = 1; // We hit a corner with two live bricks around it
									//if (j > 0) { // Make sure that there can actually be a brick to the left of it
										//if (bricks[i][j-1].lives/* || deflectedY*/) // Check if brick to the left is alive or if y-axis is already deflected											
									//}
										else
											dontDeflectX = 1; // Don't deflect it
									} //else if (dontDeflectY)
										//cornerHit = 1;
								} else if (y == bricks[i][j].y) {
									dontDeflectX = 0;
									cornerHit = 0;
								}
							} else if (x == bricks[i][j].x+bricks[i][j].width-1) { // Right side of brick
								if (y+ball.height-1 == bricks[i][j].y || y == bricks[i][j].y+bricks[i][j].height-1) { // Top or bottom right corner
									if (j+1 < BRICK_TABLE_WIDTH && bricks[i][j+1].lives) {
										if (dontDeflectY)
											cornerHit = 1; // We hit a corner with two live bricks around it
									//if (j+1 < BRICK_TABLE_WIDTH) { // Make sure that there can actually be a brick to the right of it
										//if (bricks[i][j+1].lives/* || deflectedY*/) // Check if brick to the right is alive or if y-axis is already deflected											
										else
											dontDeflectX = 1; // Don't deflect it											
									//}
									} //else if (dontDeflectY)
										//cornerHit = 1;
								} else if (y == bricks[i][j].y) {
									dontDeflectX = 0;
									cornerHit = 0;
								}
							} else if (y == bricks[i][j].y) {
								dontDeflectX = 0;
								cornerHit = 0;
							}

							if (!dontDeflectX && !cornerHit) {
								deflectedX = 1;
								ball.vector.x = -ball.vector.x;
								ball.x += 2*ball.vector.x;
							} else if (dontDeflectX) {
								gotoxy(10,2);
								printf("dontDeflectX: %d",counterX++);
							}						
						}						
					}							
				}				
			}
		}
	
		if (cornerHit) {
			gotoxy(10,20);
			printf("CornerHit: %d",counterX++);
			ball.vector.y = -ball.vector.y;
			ball.y += 2*ball.vector.y;
			ball.vector.x = -ball.vector.x;
			ball.x += 2*ball.vector.x;
		}

		gotoxyBall(ball.x,ball.y);
	}
	else if (y+ball.height-1 > striker.y) { // If you are below the striker then player must be dead	
		dead();
		gotoxy(x,y);
	}		
	else {
		if (x <= x1 || x+ball.width >= x2) {
			ball.vector.x = -ball.vector.x;
			ball.x += 2*ball.vector.x;
		}		
		if ((y <= y1) || (y+ball.height-1 == striker.y && x+ball.width > striker.x && x < striker.x+striker.width)) {						
			if (y+ball.height-1 == striker.y) { // Check if we hit the striker
				score++;
	
				distance = x+ball.width-1 - striker.x;

				if (distance < striker.width/2-1+BALL_WIDTH/2-1) // The ball hit the left side
					angle = strikerAngle[(((striker.width/2-1)+(BALL_WIDTH/2-1))-1)-distance];
				else if (distance > striker.width/2+1+BALL_WIDTH/2-1) // The ball hit the right side
				 	angle = -strikerAngle[distance-(((striker.width/2+1)+(BALL_WIDTH/2-1))+1)];
				else
					angle = 0;

				if (angle != 0) {
					ball.vector.x >>= 1; // We need to set this back again before rotating
					rotate(&ball.vector,angle);

					if (((ball.vector.y >> 12) & 0x7) == 0) { // Check if below 1/4 or approximately 15 degrees
						gotoxy(10,16);

						printf("BeforeY: ");
						printFix(ball.vector.y,4);
						ball.vector.y = 1 << 12; // 1/4
						printf(" AfterY: ");
						printFix(ball.vector.y,4);
				
						printf(" BeforeX: ");
						printFix(ball.vector.x,4);
						if (ball.vector.x & 0x80000000) // Negative
							ball.vector.x = -15864; // -(cos(arcsin(1/4)) << 14)
						else
							ball.vector.x = 15864; // cos(arcsin(1/4)) << 14
						printf(" AfterX: ");
						printFix(ball.vector.x,4);
					}

					ball.vector.x <<= 1; // The x vector needs to be twice as large do to the y-axis being twice as high on the screen
				}
				gotoxy(10,10);
				printf("%02d %02d",distance,angle);
			}
			ball.vector.y = -ball.vector.y;
			ball.y += 2*ball.vector.y;
		}
		gotoxyBall(ball.x,ball.y);
	}
	drawBigBall();
}

void setBallPos(unsigned char x, unsigned char y) {
	ball.x = (long)x << FIX14_SHIFT;
	ball.y = (long)y << FIX14_SHIFT;
}

void drawBall() {
	clearBigBall();
	if (alive && gameStarted) {
		ball.x += ball.vector.x;
		ball.y += ball.vector.y;
	}
	drawBallxy(getTerminalCoordinate(ball.x),getTerminalCoordinate(ball.y));
}

void drawStriker() {
	const unsigned char strikerStyle = 223;
	unsigned char i;
	gotoxy(striker.x,striker.y);
	for (i=0;i<striker.width;i++)
		printf("%c",strikerStyle);
}

void ballPosStriker() {
	clearBigBall();
	setBallPos(striker.x+striker.width/2-2,striker.y-2);
	drawBall();
}

void hideCursor() {
	gotoxy(300,300); // Put the cursor in the bottom right
}

void moveStriker(char dir) {
	char step = STEP_INTERVAL;
	unsigned char absStep = step;
	if (!lives) // If no lives left then return
		return;	
	if (dir < 0)
		step = -step;

	if (striker.x + step > x1 && striker.x+striker.width-absStep + step < x2) {
		striker.x += step;
		drawStriker();
		if (step < 0) // Negative
			gotoxy(striker.x+striker.width,striker.y);
		else
			gotoxy(striker.x-absStep,striker.y);
		for (; absStep > 0; absStep--)
			printf(" "); // Clear old char
		if (!gameStarted) {
			ballPosStriker();
			if (!alive)
				startGame();
		}
	}
	hideCursor();
}

void initStriker(unsigned char x, unsigned y, unsigned char width) {
	unsigned char i, dAngle, strikerZones = width/2-1+BALL_WIDTH/2-1;

	if (width > STRIKER_MAX_WIDTH)
		width = STRIKER_MAX_WIDTH;

	striker.x = x - width/2;
	striker.y = y;
	striker.width = width;
	drawStriker();

	dAngle = STRIKER_MAX_ANGLE/strikerZones; // Delta angle between striker characters

	gotoxy(10,8);
	for (i = 1; i <= strikerZones; i++) {
		strikerAngle[i-1] = dAngle*i;
		printf("%d ", strikerAngle[i-1]);
	}
}

void stopGame() {
	gameStarted = 0;
}

void initBricks() {
	unsigned char i, j;

	for (i=0;i<BRICK_TABLE_HEIGHT;i++) {
		for (j=0;j<BRICK_TABLE_WIDTH;j++) {
			brickTable[i][j] = 2; // TODO: Lav baner

			bricks[i][j].width = 14;
			bricks[i][j].height = 2;

			bricks[i][j].lives = brickTable[i][j];
			bricks[i][j].x = x1+40/*6*/ + (bricks[i][j].width+1)*j;
			bricks[i][j].y = y1+10 + (bricks[i][j].height+1)*i;
			drawBrick(&bricks[i][j]);
		}
	}
}

void startGame() {
	int startAngle;
	if (!alive) {
		alive = 1;
		if (lives == 0) {
			printTextCenter("          "); // Clear GameOver!
			initBricks(); // Draw bricks once again
			lives = NLIVES;			
			//printLives();
			score = 0;
			//printScore();
		}
		ballPosStriker();		
	} else {
		initVector(&ball.vector,1,0);
		startAngle = (millis() & 0x7F) - 192; // Calculate a "random" angle from 0-127 (0-89.3 deg) and then subtract 192 (135 deg)

		gotoxy(10,6);
		printf("Start angle: %02d",(startAngle*360)/512);
		
		rotate(&ball.vector, startAngle); 
		
		printf(" ");
		printFix(ball.vector.x,4);
		printf(" ");
		printFix(ball.vector.y,4);

		ball.vector.x <<= 1; // The x vector needs to be twice as large do to the y-axis being twice as high on the screen
		gameTimer = 0;
		gameStarted = 1;
		updateGame();
	}
}

void updateGame() {
	unsigned char speed = DEFAULT_DIFFICULTY-score;
	if (speed < MAX_DIFFICULTY)
		speed = MAX_DIFFICULTY;
	if (millis() - gameTimer > speed) {
		if (gameStarted) {
			gameTimer = millis();
			drawBall();
		}
		hideCursor();
		//gotoxy(10,14);
		//printf("Speed %d",DEFAULT_DIFFICULTY-score);
	}	
}

void initBall() {
	setBallPos(striker.x+striker.width/2-2,striker.y-2); // Initialize the ball position to the striker position, as ballPosStriker will clear this
	ball.width = BALL_WIDTH; // This has to be even
	ball.height = BALL_HEIGHT;
	initVector(&ball.vector,0,0);
}

void initReflexBall(unsigned char newX1, unsigned char newY1, unsigned char newX2, unsigned char newY2, char style) {
	unsigned char leftTop, rightTop, leftBot, rightBot, verSide, horSide, leftCross, rightCross;

	x1 = newX1;
	y1 = newY1;
	x2 = newX2;
	y2 = newY2;

	if (style) { // Bold
		leftTop = 201;
		rightTop = 187;
		leftBot = 200;
		rightBot = 188;
		verSide = 186;
		horSide = 205;
		leftCross = 185;
		rightCross = 204;
	} else { // Normal
		leftTop = 218;
		rightTop = 191;
		leftBot = 192;
		rightBot = 217;
		verSide = 179;
		horSide = 196;
		leftCross = 180;
		rightCross = 195;
	}

	gotoxy(x1,y1);
	drawTopBot(x1,x2,leftTop,rightTop,horSide);	   
	drawSides(x1,y1,x2,y2,verSide);

	initStriker((x2-x1)/2+x1,y2-1,10); // The width of the striker should always be even
	initBall(); // Initialize to striker position

	ballPosStriker();
	alive = 1;
	gameStarted = 0;

	lives = NLIVES;
	//printLives();
	score = 0;
	//printScore();

	initBricks();
}
