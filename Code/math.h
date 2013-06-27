#ifndef _math_h_
#define _math_h_

#define FIX14_SHIFT 14

#define FIX14_MULT(a, b) ((a*b) >> FIX14_SHIFT)
#define FIX14_DIV(a, b) (a << FIX14_SHIFT / b)

typedef struct {
	long x,y;
} TVector;

// Public
long expand(long input);
void printNumber(long input, unsigned char decimalBits, unsigned char decimal);
void printFix(long input, unsigned char decimal);
long sin(int val);
long cos(int val);
void initVector(TVector* v, long x, long y);
void rotate(TVector* v, int val);
void printVector(TVector* v);

#endif