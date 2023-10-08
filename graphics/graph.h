//#include "C:\gcc\x86_64-w64-mingw32\include\GL\glut.h"
#include "token.h"
typedef unsigned char byte;

#define DIB_RGB_COLORS      0 

void GRscale(_Bool);
void GRmove(byte);

void GRinit();
int GRdraw();

void GRfillMap();
void GRfillGraph();
void GRline();
void GRsetPix(double,double,byte,byte,byte,byte);