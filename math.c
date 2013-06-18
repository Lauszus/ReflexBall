#include <eZ8.h>             // special encore constants, macros and flash routines
#include <sio.h>             // special encore serial i/o routines
#include "math.h"
#include "lut.h"

long expand(long input) {
	return input << 2; // Convert an 18.14 to 16.16
}

void printNumber(long input,  unsigned char decimalBits, unsigned char decimal) {
	int i;
	unsigned long pow = 10, mask = 0xFFFFFFFF, result, output;

	if ((input & 0x80000000) != 0) {
		printf("-");
		input = -input;
	}

	if (decimal > 5)
		decimal = 5;

	for (i=1;i<decimal;i++)
		pow *= 10;

	mask >>= (32 - decimalBits);
	result = pow * (unsigned long)(input & mask);
	output = result >> decimalBits;
	output += (result >> (decimalBits-1)) & 0x1; // Round correctly

	printf("%ld.%0*ld", input >> decimalBits, decimal, output);
}

void printFix(long input, unsigned char decimal) {
	printNumber(input,FIX14_SHIFT,decimal);
}

long sin(int val) {
	val -= val & 0xFFFFFE00;
	return SIN[val]; // & 0x1FF
}

long cos(int val) {
	return sin(val + 128);
}

void initVector(TVector* v, long x, long y) {
	v->x = x << FIX14_SHIFT;
	v->y = y << FIX14_SHIFT;
}

void rotate(TVector* v, int val) {
	long sinVal, cosVal, tempX;
 	sinVal = sin(val);
	cosVal = cos(val);
	tempX = v->x;

	v->x = FIX14_MULT(v->x,cosVal) - FIX14_MULT(v->y,sinVal);
	v->y = FIX14_MULT(tempX,sinVal) + FIX14_MULT(v->y,cosVal);
}

void printVector(TVector* v) {
	printf("(");
	printFix(v->x,4);
	printf(",");
	printFix(v->y,4);
	printf(")\n");
}