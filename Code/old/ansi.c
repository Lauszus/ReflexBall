void drawBanner(unsigned char x1, unsigned char y1, unsigned char left, unsigned char right, char* title) {
	gotoxy(x1+1,y1);
	printf("%c",left);
	reverse(1);
	printf("  %s  ",title);
	reverse(0);
	printf("%c",right);
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