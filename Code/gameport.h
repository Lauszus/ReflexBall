#ifndef _gameport_h_
#define _gameport_h_

void initGameport();
unsigned char getGameportButtons();
unsigned int readADC(unsigned char channel);
char readSteeringWheel();

#endif
