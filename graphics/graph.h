//#include "C:\gcc\x86_64-w64-mingw32\include\GL\glut.h"
#include "token.h"

typedef unsigned char byte;
typedef byte BYTE;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned short WORD;
typedef void *PVOID;
typedef PVOID HANDLE;
typedef HANDLE HDC;
typedef struct tagRGBQUAD {
  BYTE    rgbBlue; 
  BYTE    rgbGreen; 
  BYTE    rgbRed; 
  BYTE    rgbReserved; 
} RGBQUAD; 

typedef struct tagBITMAPINFOHEADER{
  DWORD  biSize; 
  LONG   biWidth; 
  LONG   biHeight; 
  WORD   biPlanes; 
  WORD   biBitCount; 
  DWORD  biCompression; 
  DWORD  biSizeImage; 
  LONG   biXPelsPerMeter; 
  LONG   biYPelsPerMeter; 
  DWORD  biClrUsed; 
  DWORD  biClrImportant; 
} BITMAPINFOHEADER, *PBITMAPINFOHEADER; 

typedef struct tagBITMAPINFO { 
  BITMAPINFOHEADER bmiHeader; 
  RGBQUAD          bmiColors[1]; 
} BITMAPINFO, *PBITMAPINFO;

#define BI_RGB        0L
#define DIB_RGB_COLORS      0 

void GRscale(_Bool);
void GRmove(byte);

void GRinit();
int GRdraw();

void GRfillMap();
void GRfillGraph();
void GRline();
void GRsetPix(double,double,byte,byte,byte,byte);