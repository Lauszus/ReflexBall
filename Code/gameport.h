#ifndef _gameport_h_
#define _gameport_h_

// Public
void initGameport();
unsigned char getGameportButtons();
char readSteeringWheel();

// Private
unsigned int readADC(unsigned char channel);

#endif