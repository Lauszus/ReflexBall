#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "startScreen.h"
#include "reflexball.h"
#include "ansi.h"
#include "LED.h"

rom const char titleAscii[11][127] = {
	"                                                                               )                           (                ) ",
	"                                                                            ( /(         (                 )\\ )          ( /( ", 
	"                                                                           )\\) )(        ))\\       (      (()/(      (   )\\())",
	"                                                                          ((_) ()\\    ((()(_)(   ( )\\      /(_))     )\\ ((_)\\ ",
	"                                                                          (_()_((_)    )\\ __ )\\  )((_)    (__))     ((_)  ((_)",
	"                                                                          /\"      \\    (_)\"\"\\())((_) |    |\"  |     |\"  \\/\"  |",
	"__________        _____.__                __________        .__  .__     |:        |    /    \\_) ||  |    ||  |      \\   \\  / ",
	"\\______   \\ _____/ ____\\  |   ____ ___  __\\______   \\_____  |  | |  |    |_____/   )   /\' /\\  \\  |:  |    |:  |       \\\\  \\/  ",
	" |       _// __ \\   __\\|  | _/ __ \\\\  \\/  /|    |  _/\\__  \\ |  | |  |     //      /   //  __\'  \\  \\  |___  \\  |___    /   /   ",
	" |    |   \\  ___/|  |  |  |_\\  ___/ >    < |    |   \\ / __ \\|  |_|  |__  |:  __   \\  /   /  \\\\  \\( \\_|:  \\( \\_|:  \\  /   /    ",
	" |____|_  /\\___  >__|  |____/\\___  >__/\\_ \\|______  /(____  /____/____/  |__|  \\___)(___/    \\___)\\_______)\\_______)|___/     ",
};

rom const char menuAscii[10][78] = {
	"                  /$$      /$$ /$$$$$$$$ /$$   /$$ /$$   /$$                 ",
	"                 | $$$    /$$$| $$_____/| $$$ | $$| $$  | $$                 ",
	"                 | $$$$  /$$$$| $$      | $$$$| $$| $$  | $$                 ",
	"                 | $$ $$/$$ $$| $$$$$   | $$ $$ $$| $$  | $$                 ",
	"                 | $$  $$$| $$| $$__/   | $$  $$$$| $$  | $$                 ",
	"                 | $$\\  $ | $$| $$      | $$\\  $$$| $$  | $$                 ",
	"                 | $$ \\/  | $$| $$$$$$$$| $$ \\  $$|  $$$$$$/                 ",
	"                 |__/     |__/|________/|__/  \\__/ \\______/                  ",
	" /$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
	"|___________________________________________________________________________/",
};

rom const char easyAscii[8][24] = {
	" ______                ",
 	"|  ____|               ",
 	"| |__   __ _ ___ _   _ ",
 	"|  __| / _` / __| | | |",
 	"| |___| (_| \\__ \\ |_| |",
 	"|______\\__,_|___/\\__, |",
    "                  __/ |",
    "                 |___/ ",
};

rom const char mediumAscii[6][38] = {
	" __  __          _ _                 ",
	"|  \\/  |        | (_)                ",
	"| \\  / | ___  __| |_ _   _ _ __ ___  ",
	"| |\\/| |/ _ \\/ _` | | | | | \'_ ` _ \\ ",
	"| |  | |  __/ (_| | | |_| | | | | | |",
	"|_|  |_|\\___|\\__,_|_|\\__,_|_| |_| |_|",
};

rom const char hardAscii[6][25] = {
	" _    _               _ ",
	"| |  | |             | |",
	"| |__| | __ _ _ __ __| |",
	"|  __  |/ _` | \'__/ _` |",
	"| |  | | (_| | | | (_| |",
	"|_|  |_|\\__,_|_|  \\__,_|",
};

unsigned char strlen_rom(rom const char *string) {
	unsigned char length = 0;
	while ((char)*string++ != '\0')
		length++;
	return length;
}

void printAscii(rom const char *str, unsigned char size) {	
	unsigned char i;

	for (i=1;i<=size;i++) {
		getSavedCursor();
		moveCursor(DOWN,i);
		while ((char)*str != '\0')
			printf("%c",*str++);
		str++;		
	}
	getSavedCursor();
	moveCursor(DOWN,size);
	saveCursor();
}

void printAsciiXY(rom const char *str, unsigned char size, unsigned char x, unsigned char y) {
	unsigned char i, length = strlen_rom(str);	
	for (i=0;i<size;i++) {
		gotoxy(x,y+i);
		while ((char)*str != '\0')
			printf("%c",*str++);
		str++;
	}
	moveCursor(BACK,length);
	saveCursor();
}

void printMenu(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, char style) {
	printAsciiXY(titleAscii[0],sizeof(titleAscii)/sizeof(titleAscii[0]),(x1+x2)/2-strlen_rom(titleAscii[0])/2,y1+5);

	window((x1+x2)/2-60, (y1+y2)/2-20, (x1+x2)/2+60, (y1+y2)/2+20, "ReflexBall Rally", style);
	printAsciiXY(menuAscii[0],sizeof(menuAscii)/sizeof(menuAscii[0]),(x1+x2)/2-strlen_rom(menuAscii[0])/2,(y1+y2)/2-15);

	moveCursor(FORWARD,strlen_rom(menuAscii[0])/2-strlen_rom(easyAscii[0])/2);
	moveCursor(DOWN,1);
	saveCursor();

	printAscii(easyAscii[0],sizeof(easyAscii)/sizeof(easyAscii[0]));
	printAscii(mediumAscii[0],sizeof(mediumAscii)/sizeof(mediumAscii[0]));
	moveCursor(DOWN,1);
	saveCursor();
	printAscii(hardAscii[0],sizeof(hardAscii)/sizeof(hardAscii[0]));
}