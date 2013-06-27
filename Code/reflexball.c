#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "reflexball.h"
#include "time.h"
#include "levels.h"
#include "LED.h"
#include "ascii.h"
#include "asciidisplay.h"

Ball ball;
Striker striker;
Brick bricks[BRICK_TABLE_HEIGHT][BRICK_TABLE_WIDTH];
unsigned char strikerAngle[STRIKER_MAX_WIDTH/2-1+BALL_WIDTH/2-1];
unsigned int bricksLives; // Stores the total number of lives of the bricks

unsigned long gameTimer;
unsigned char gameStarted = 0, alive;

unsigned char x1, y1, x2, y2;

unsigned int score, levelScore; // The total score and the score obtained in this level
unsigned char divider; // This is the difficulty set in the beginning
unsigned char strikerWidth; // This is the striker width determent from the selected difficulty

unsigned char lives, level;
volatile char runOnceBuf[50], ledBuf[50]; // We need this global buffer as the LED code creates a pointer to the memory location

char drawCounter = 0;
char drawCounterMax = 1;
char drawBallNow;

long lastX, lastY;

unsigned char restartGame; // True if the user have won the game

void printLevel() {
	gotoxy(x2-35,y2);
	printf("Level: %d", level+1);
}
void printLives() {
	gotoxy(x2-25,y2);
	printf("Lives: %d", lives);	
}
void printScore() {
	gotoxy(x2-15,y2);
	printf("Score: %04d$", score);
}

void showScoreLED() {
	sprintf(ledBuf,"%04d",score);
	LEDsetString(ledBuf);
}
void scrollLiveInGameLED() {
	sprintf(ledBuf,"%04d",score);
	sprintf(runOnceBuf,"%s$ Lives:%d£ %s ", ledBuf, lives, ledBuf); // We have to put a space behind as the LED routine aborts when it loaded the last character in the 5th digit
	
	LEDRunOnce(runOnceBuf,ledBuf);
}
void scrollLevelUp() {
	sprintf(ledBuf,"%04d", score);
	sprintf(runOnceBuf,"%s$ Level up!§ Level:%d Lives:%d£ %s ", ledBuf, level+1, lives, ledBuf); // We have to put a space behind as the LED routine aborts when it loaded the last character in the 5th digit
	
	LEDRunOnce(runOnceBuf,ledBuf);
}
void scrollAll() {
	sprintf(ledBuf,"%04d",score);
	sprintf(runOnceBuf,"%s$ Level:%d Lives:%d£ %s ", ledBuf, level+1, lives, ledBuf); // We have to put a space behind as the LED routine aborts when it loaded the last character in the 5th digit
	
	LEDRunOnce(runOnceBuf,ledBuf);
}

void dead() {
	if(--lives == 0) {
		delay_ms(1000); // Wait a bit before showing Game Over ASCII string
		sprintf(ledBuf,"%04d$ Game Over! Score:", score);
		LEDsetString(ledBuf);
		showGameOver();
		restartGame = 1;
		return;
	} else {
		scrollLiveInGameLED();
		printLives();
	}

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

void clearBigBall(long x, long y) {
	gotoxyBall(x,y);
	printf("    ");
	gotoxyBall(x,y + ((long)1 << FIX14_SHIFT));
	printf("    ");
}

void drawBigBall() {
	const unsigned char top = 238, bottom = 95, slash = '/', backSlash = '\\';

	if ((++drawCounter != drawCounterMax && !drawBallNow) || !alive) // Check if we should skip the draweing in this iteration or if the game is not running
		return;
	drawCounter = 0;
	drawBallNow = 0;

	clearBigBall(lastX,lastY);

	lastX = ball.x;
	lastY = ball.y;

	gotoxyBall(ball.x,ball.y);
	printf("%c%c%c%c",slash,top,top,backSlash);
	gotoxyBall(ball.x,ball.y + ((long)1 << FIX14_SHIFT));
	printf("%c%c%c%c",backSlash,bottom,bottom,slash);
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
			else if (brick->lives == 4)
				brickStyle = 219;
			else // Invisible
				brickStyle = ' ';
			printf("%c",brickStyle);			
		}
	}
}

void checkIteration(unsigned char x, unsigned char y) {
	unsigned char i, j, dontDeflectX = 0, dontDeflectY = 0, deflectedX = 0, deflectedY = 0;
	char distance; // Distance from center to ball position on striker
	int angle;

	if ((x > x1 && x+ball.width < x2 && y > y1 && y+ball.height-1 < y2-1) && gameStarted) {
		for (i=0;i<BRICK_TABLE_HEIGHT;i++) {
			for (j=0;j<BRICK_TABLE_WIDTH;j++) {
				if (!bricks[i][j].lives) // Skip if lives == 0
					continue;
	
				if (y-1 == bricks[i][j].y+bricks[i][j].height-1 || y+ball.height-1 == bricks[i][j].y-1 || x+ball.width-1 == bricks[i][j].x-1 || x-1 == bricks[i][j].x+bricks[i][j].width-1)
					drawBallNow = 1; // Make sure it draws the ball in this iteration

				// Check if the ball hit a brick
				if (y <= bricks[i][j].y+bricks[i][j].height-1 && y+ball.height-1 >= bricks[i][j].y && x+ball.width-1 >= bricks[i][j].x && x <= bricks[i][j].x+bricks[i][j].width-1) {
					score++;
					levelScore++;
					printScore();
					showScoreLED();
					
					// Check if the ball hit the top or the bottom of the brick
					if (y <= bricks[i][j].y+bricks[i][j].height-1 && y+ball.height-1 >= bricks[i][j].y) {												
			  		    if (!deflectedY) {
							if (x+ball.width-1 == bricks[i][j].x || x+ball.width-2 == bricks[i][j].x || x == bricks[i][j].x+bricks[i][j].width-1 || x == bricks[i][j].x+bricks[i][j].width-2) { // Left or right side
								if (y+ball.height-1 >= bricks[i][j].y && y <= bricks[i][j].y) { // Top left or right corner on brick
									if (i > 0 && bricks[i-1][j].lives) { // Make sure that there can actually be a brick above it
										dontDeflectY = 1; // Don't deflect it
										//gotoxy(x2-20,2);
										//printf("Alive above! %04d",dontCounter++);
									}
								}
								if (y <= bricks[i][j].y+bricks[i][j].height-1 && y+ball.height-1 >= bricks[i][j].y+bricks[i][j].height-1) { // Bottom left or right corner on brick
									if (i+1 < BRICK_TABLE_HEIGHT && bricks[i+1][j].lives) { // Make sure that there can actually be a brick underneath it
										dontDeflectY = 1; // Don't deflect it
										//gotoxy(x2-20,2);
										//printf("Alive below! %04d",dontCounter++);
									}
								}
							} else if (y >= bricks[i][j].y && y+ball.height-1 <= bricks[i][j].y+bricks[i][j].height-1) // Check if we hit the brick directly from the side
								dontDeflectY = 1;

							if (y+ball.height-1 >= bricks[i][j].y && y <= bricks[i][j].y) { // Top of brick
								if (ball.vector.y & 0x80000000) { // Negative
									dontDeflectY = 1; // Don't deflect it
									//gotoxy(x2-20,6);
									//printf("Top negative! %04d",dontCounter++);
								}
							}
							if (y <= bricks[i][j].y+bricks[i][j].height-1 && y+ball.height-1 >= bricks[i][j].y+bricks[i][j].height-1) { // Bottom of brick
								//gotoxy(x2-20,6);
								//printf("Bottom ");
								if (!(ball.vector.y & 0x80000000)) { // Positive
									dontDeflectY = 1; // Don't deflect it
									//printf("positive! %04d",dontCounter++);
								} /*else
									printf("negative! %04d",dontCounter++);*/
							}
							
							if (!dontDeflectY) {
								deflectedY = 1;
								ball.vector.y = -ball.vector.y;
								ball.y += 2*ball.vector.y;
							} /*else if (dontDeflectY) {
								gotoxy(10,4);
								printf("dontDeflectY: %d",dontCounter++);
							}*/
						}
					}
					// Check if the ball hit one of the sides of the brick
					if (x+ball.width-1 == bricks[i][j].x || x+ball.width-2 == bricks[i][j].x || x == bricks[i][j].x+bricks[i][j].width-1 || x == bricks[i][j].x+bricks[i][j].width-2) {						
						if (!deflectedX) {
							if ((y+ball.height-1 >= bricks[i][j].y && y <= bricks[i][j].y) || (y <= bricks[i][j].y+bricks[i][j].height-1 && y+ball.height-1 >= bricks[i][j].y+bricks[i][j].height-1)) { // Top or bottom of brick
								if (x+ball.width-1 == bricks[i][j].x || x+ball.width-2 == bricks[i][j].x) { // Top or bottom left side
									if (j > 0 && bricks[i][j-1].lives) { // Check if there is one alive to the left of the brick
										dontDeflectX = 1; // Don't deflect it
										//gotoxy(x2-20,4);
										//printf("Alive left! %04d",dontCounter++);
									}
								}
								if (x == bricks[i][j].x+bricks[i][j].width-1 || x == bricks[i][j].x+bricks[i][j].width-2) { // Top or bottom right side
									if (j+1 < BRICK_TABLE_WIDTH && bricks[i][j+1].lives) { // Check if there is one alive to the right of the brick
										dontDeflectX = 1; // Don't deflect it
										//gotoxy(x2-20,4);
										//printf("Alive right! %04d",dontCounter++);
									}
								}
							}
							
							if (x+ball.width-1 == bricks[i][j].x || x+ball.width-2 == bricks[i][j].x) { // Left side
								if (ball.vector.x & 0x80000000) { // Negative
									dontDeflectX = 1; // Don't deflect it
									//gotoxy(x2-20,8);
									//printf("Left negative! %04d",dontCounter++);
								}
							}
							if (x == bricks[i][j].x+bricks[i][j].width-1 || x == bricks[i][j].x+bricks[i][j].width-2) { // Right side
								if (!(ball.vector.x & 0x80000000)) { // Positive
									dontDeflectX = 1; // Don't deflect it
									//gotoxy(x2-20,8);
									//printf("Right positive! %04d",dontCounter++);
								}
							}
	
							if (y != bricks[i][j].y) { // It hasn't hit the brick directly from the side
								if (y < bricks[i][j].y) { // Top
									if (!(ball.vector.y & 0x80000000) && !(i > 0 && bricks[i-1][j].lives)) { // Check if it is positive and make sure there is no brick above
										dontDeflectX = 1; // Don't deflect it
										//gotoxy(x2-20,14);
										//printf("Top none above! %04d",dontCounter++);
									}
								} else { // Bottom
									if (ball.vector.y & 0x80000000 && !(i+1 < BRICK_TABLE_HEIGHT && bricks[i+1][j].lives)) { // Check if it is negative and make sure there is no brick below
										dontDeflectX = 1; // Don't deflect it
										//gotoxy(x2-20,14);
										//printf("Bottom none below! %04d",dontCounter++);
									}
								}								
							}

							if (!dontDeflectX) {
								deflectedX = 1;
								ball.vector.x = -ball.vector.x;
								ball.x += 2*ball.vector.x;
							} /*else if (dontDeflectX) {
								gotoxy(10,2);
								printf("dontDeflectX: %d",dontCounter++);
							}*/
						}						
					}
					bricks[i][j].lives--;
					bricksLives--;				
					drawBrick(&bricks[i][j]);
					if (!bricksLives) {
						levelUp();
						return;
					}
					ball.vector.x >>= 1; // We need to set this back again before rotating
					angle = (int)(millis() & 0x7) - 3; // Pseudo random number from -3 to 4
					//gotoxy(x1+10,y2-10);
					//printf("Angle: %02d", angle);
					rotate(&ball.vector, angle); // Slightly rotate the ball from -3 to 4, to make the game more exciting
					ball.vector.x <<= 1; // The x vector needs to be twice as large due to the y-axis being twice as high on the screen
				}			
			}
		}
	
		if (dontDeflectX && dontDeflectY) {
			//gotoxy(x2-20,12);
			//printf("CornerHit: %d",dontCounter++);
			ball.vector.y = -ball.vector.y;
			ball.y += 2*ball.vector.y;
			ball.vector.x = -ball.vector.x;
			ball.x += 2*ball.vector.x;
		}
	}
	else if (((y+ball.height-1 > striker.y) || (y+ball.height-1 == striker.y && (x+ball.width <= striker.x || x >= striker.x+striker.width))) && gameStarted) { // If you are below the striker then player must be dead	
		drawBallNow = 1;
		drawBigBall();
		dead();
		return;
	}
	else if (gameStarted) {
		if (x <= x1 || x+ball.width >= x2) {
			ball.vector.x = -ball.vector.x;
			ball.x += 2*ball.vector.x;
		}		
		if ((y <= y1) || (y+ball.height-1 == striker.y && x+ball.width > striker.x && x < striker.x+striker.width)) {
			if (y+ball.height-1 == striker.y) { // Check if we hit the striker
				printScore();
				showScoreLED();
	
				distance = x+ball.width-1 - striker.x;

				if (distance < striker.width/2-1+BALL_WIDTH/2-1) // The ball hit the left side
					angle = strikerAngle[(((striker.width/2-1)+(BALL_WIDTH/2-1))-1)-distance];
				else if (distance > striker.width/2+1+BALL_WIDTH/2-1) // The ball hit the right side
				 	angle = -(int)strikerAngle[distance-(((striker.width/2+1)+(BALL_WIDTH/2-1))+1)]; // IMPORTANT: remember to cast to int before the minus sign
				else
					angle = 0;

				if (angle != 0) {
					/*gotoxy(10,20);
					printf("                  ");
					gotoxy(10,20);
					printf("Rotating: %d", angle);*/


					ball.vector.x >>= 1; // We need to set this back again before rotating
					rotate(&ball.vector,angle);

					if (((ball.vector.y >> 12) & 0x7) == 0) { // Check if below 1/4 or approximately 15 degrees
						/*gotoxy(10,16);
						printf("BeforeY: ");
						printFix(ball.vector.y,4);*/
						ball.vector.y = 1 << 12; // 1/4
						/*printf(" AfterY: ");
						printFix(ball.vector.y,4);*/
				
						/*printf(" BeforeX: ");
						printFix(ball.vector.x,4);*/
						if (ball.vector.x & 0x80000000) // Negative
							ball.vector.x = -(long)15864; // -(cos(arcsin(1/4)) << 14)
						else
							ball.vector.x = 15864; // cos(arcsin(1/4)) << 14
						/*printf(" AfterX: ");
						printFix(ball.vector.x,4);*/
					}

					ball.vector.x <<= 1; // The x vector needs to be twice as large due to the y-axis being twice as high on the screen
				}
				/*gotoxy(10,10);
				printf("%02d %02d",distance,angle);*/
			}
			ball.vector.y = -ball.vector.y;
			ball.y += 2*ball.vector.y;
		}
	}

	if (x == x1+1 || x+ball.width == x2-1 || y == y1+1 || (y+ball.height-1 == striker.y-1 && x+ball.width > striker.x && x < striker.x+striker.width)) // Check if we are next to the sides, top or striker
		drawBallNow = 1; // If so then draw the ball in this iteration

	drawBigBall();	
}

void setBallPos(unsigned char x, unsigned char y) {
	ball.x = (long)x << FIX14_SHIFT;
	ball.y = (long)y << FIX14_SHIFT;
}

void iterate() {
	if (alive && gameStarted) {
		ball.x += ball.vector.x;
		ball.y += ball.vector.y;
	}
	checkIteration(getTerminalCoordinate(ball.x),getTerminalCoordinate(ball.y));
}

void drawStriker() {
	const unsigned char strikerStyle = 223;
	unsigned char i;
	gotoxy(striker.x,striker.y);
	if (divider == 1) // Blink striker if Chuck Norris mode is on
		blink(1);
	for (i=0;i<striker.width;i++)
		printf("%c",strikerStyle);
	if (divider == 1)
		blink(0);
}

void ballPosStriker() {
	setBallPos(striker.x+striker.width/2-ball.width/2,striker.y-ball.height);
	iterate();
	drawStriker(); // Redraw striker in case the ball clears part of the stikers
}

void moveStriker(char dir) { // Take care of moving the striker left or right
	char absDir = dir;
	if (!lives) // If no lives left then return
		return;	
	if (dir < 0)
		absDir = -dir;

	if ((int)striker.x + dir <= x1)
		striker.x = x1+1;
	else if (striker.x+striker.width-1 + dir >= x2)
		striker.x = x2-striker.width;
	else
		striker.x += dir;

	if (dir < 0) // Left
		gotoxy(striker.x+striker.width,striker.y);
	else // Right
		gotoxy(striker.x-dir,striker.y);

	for (; absDir > 0; absDir--)
		printf(" "); // Clear old char

	drawStriker();

	if (!gameStarted) {
		ballPosStriker();
		if (!alive)
			startGame();
	}
}

void initStriker(unsigned char x, unsigned y, unsigned char width) {
	unsigned char i, dAngle, strikerZones;

	if (width > STRIKER_MAX_WIDTH)
		width = STRIKER_MAX_WIDTH;

	strikerZones = width/2-1+BALL_WIDTH/2-1; // The striker is split up into zones to the left and right of the striker center

	striker.x = x - width/2;
	striker.y = y;
	striker.width = width;
	drawStriker();

	dAngle = STRIKER_MAX_ANGLE/strikerZones; // Delta angle between striker characters

	//gotoxy(10,8);
	for (i = 1; i <= strikerZones; i++) {
		strikerAngle[i-1] = dAngle*i; // Fill in the array - the longer distance from the center, the larger angle it will be reflected with
		//printf("%d ", strikerAngle[i-1]);
	}
}

void stopGame() {
	gameStarted = 0;
}

void initBricks(char clear) {
	unsigned char i, j;

	bricksLives = 0; // Reset

	for (i=0;i<BRICK_TABLE_HEIGHT;i++) {
		for (j=0;j<BRICK_TABLE_WIDTH;j++) {
			bricks[i][j].width = 14;
			bricks[i][j].height = 2;

			bricks[i][j].lives = levels[level][i][j];
			bricksLives += bricks[i][j].lives;
			bricks[i][j].x = x1+6 + (bricks[i][j].width+1)*j;
			bricks[i][j].y = y1+3 + (bricks[i][j].height+1)*i;
			if (bricks[i][j].lives || clear)
				drawBrick(&bricks[i][j]);
		}
	}
}

void startGame() {
	int startAngle;
	if (!alive) {
		alive = 1;
		if (lives == 0) {						
			score = 0;
			levelScore = 0;
			showScoreLED();
			initReflexBall(x1,y1,x2,y2,1);

			gameTimer = 0;
			printLives();
			printScore();			
			printLevel();
		}
		ballPosStriker();		
	} else if (!gameStarted) {
		initVector(&ball.vector,1,0);
		startAngle = (millis() & 0x7F) - 192; // Calculate a "random" angle from 0-127 (0-89.3 deg) and then subtract 192 (135 deg)

		//gotoxy(10,6);
		//printf("Start angle: %02d",(startAngle*360)/512);
		
		rotate(&ball.vector, startAngle);
		
		/*printf(" ");
		printFix(ball.vector.x,4);
		printf(" ");
		printFix(ball.vector.y,4);*/

		ball.vector.x <<= 1; // The x vector needs to be twice as large due to the y-axis being twice as high on the screen
		gameTimer = 0;
		gameStarted = 1;
		updateGame();
	}
}

void updateGame() {
	int speed = DEFAULT_DIFFICULTY-levelScore/divider; // Calculate teh speed using the score obtained in the current level and the divider set in the start up menu
	if (speed < MAX_DIFFICULTY || divider == 1) // Limit maximum speed and set maximum speed as default if Chuck Norris mode is enabled
		speed = MAX_DIFFICULTY;
		
	if (speed < UART_MAX_SPEED) // If it gets below this value, we will only draw it every second time or if it hits an object, this is because the UART can not send the characters fast enough
		drawCounterMax = 2;
	else
		drawCounterMax = 1;

	if (millis() - gameTimer > speed && gameStarted) {
		gameTimer = millis();
		iterate();
	}
}

void initBall() {
	setBallPos(striker.x+striker.width/2-2,striker.y-2); // Initialize the ball position to the striker position, as ballPosStriker will clear this
	ball.width = BALL_WIDTH; // This has to be even
	ball.height = BALL_HEIGHT;
	initVector(&ball.vector,0,0);
}
void drawLevel(char clear) {
	unsigned char i;

	gotoxy(striker.x,striker.y);
	for (i=0;i<striker.width;i++)
		printf(" "); // Clear old striker
	initStriker((x2-x1)/2+x1,y2-1,strikerWidth); // The width of the striker should always be even

	initBall(); // Initialize to striker position
	
	alive = 1;
	gameStarted = 0;
	ballPosStriker();

	printLives();
	printScore();	
	printLevel();

	gameTimer = 0;

	initBricks(clear);
}

void levelUp() {
	level++;	
	if (level >= sizeof(levels)/sizeof(levels[0])) { // Check if the user won the game
		showWon();
		if (divider != 1) { // If not in Chuck Norris mode, then the user wins the game
			restartGame = 1;
			return;
		}
		initReflexBall(x1,y1,x2,y2,1);
		return;
	}	
	lives += 2;
	
	levelScore = 0;
	scrollLevelUp();
	drawLevel(1);
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

	lastX = (long)((x1+x2)/2) << FIX14_SHIFT; // Sets the last ball coordinates just above striker, so it clears the ball correctly the first time
	lastY = (long)(y2-1-BALL_HEIGHT) << FIX14_SHIFT;

	clrscr(); // Clear the screen
	
	// Put the title in the top of the screen - this is done when animateTitle() is called inside updateGame()
	printAsciiXY(titleAscii1[0],sizeof(titleAscii1)/sizeof(titleAscii1[0]),(x1+x2)/2-strlen_rom(titleAscii1[0])/2,y1/2-(sizeof(titleAscii1)/sizeof(titleAscii1[0]))/2);

	drawSides(x1,y1,x2,y2,verSide);	
	drawTopBot(x1,y1,x2-x1-1,leftTop,rightTop,horSide);
	
	restartGame = 0;
	level = 0;
	score = 0;
	levelScore = 0;
	lives = NLIVES;
	drawLevel(0);
	scrollAll();
}