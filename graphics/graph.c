#include "graph.h"
extern double fabs(double);
extern int printf(char*,...);
extern int GetSystemMetrics(int);
extern void *malloc(int);
extern struct BinParce *top;
extern HDC hdc;
extern void testfunc(double,double,byte,byte,byte,byte);

byte* colors;
byte* points;
double Xmax,Xmin,Ymax,Ymin;
unsigned int width,heigh;
BITMAPINFOHEADER BMIH;
BITMAPINFO BMI={0,0};

void GRinit()
{
    width=GetSystemMetrics(0);
    heigh=GetSystemMetrics(1);
        BMIH.biSize=sizeof(BITMAPINFOHEADER);
        BMIH.biWidth=width;
        BMIH.biHeight=heigh;
        BMIH.biPlanes=1;
        BMIH.biBitCount=24;
        BMIH.biCompression=BI_RGB;
        BMIH.biClrImportant=0;
        BMIH.biClrUsed=0;
        BMIH.biSizeImage=0;
        BMIH.biYPelsPerMeter=0;
        BMIH.biXPelsPerMeter=0;
        
        BMI.bmiHeader=BMIH;
        Ymax=10;
        Ymin=-Ymax;
        Xmax =Ymax*((float)width/(float)heigh);
        Xmin=-Xmax;
    colors = malloc(width*3*(heigh+1));
    memset(colors,0xFFFFFF,width*3*heigh);
}

void GRscale(_Bool dir)
{
if(dir){
    Xmax*=1.1;Ymax*=1.1;Xmin*=1.1;Ymin*=1.1;}
else{
    Xmax*=0.9;Ymax*=0.9;Xmin*=0.9;Ymin*=0.9;}
}

int GRdraw()
{
return SetDIBitsToDevice(hdc,0,0,width,heigh,0,0,0,heigh,colors,&BMI,DIB_RGB_COLORS);
}

void GRfillMap()
{
 double x,y;
unsigned int xpix,ypix;
 double step = 0.5;
 memset(colors,0xEEEEEE,width*3*heigh);

y=Ymin,x=Xmin;
double step1 = (Xmax-Xmin)/(double)width;

while(y<Ymax)
{
    for(double X = Xmin+step1; X < Xmax; X+=step1)
      testfunc(X,y,0xBB, 0xBB, 0xBB, 0);
    y+=step;
}

step1 = (Ymax-Ymin)/(double)heigh;
while(x<Xmax)
{
    for(double Y = Ymin+step1; Y < Ymax; Y+=step1)
      testfunc(x,Y,0xBB, 0xBB, 0xBB, 0);
    x+=step;
}

if(Xmin<=0 && Xmax>=0)
{
   step = (Ymax-Ymin)/(double)heigh;
  for (double Y = Ymin+step; Y < Ymax; Y+=step)
  testfunc(0,Y,0,0,0xFF,1);
}

if(Ymin<=0 && Ymax>=0)
{
  step = (Xmax-Xmin)/(double)width;
  for (double X=Xmin+step;X<Xmax;X+=step)
    testfunc(X,0,0,0,0xFF,1);
     
}
//printf("Xpixel=%d Ypixel=%d\n",xpix/3,ypix);
}

void GRmove(byte side)
{
   double step=(Xmax-Xmin)/1000.0;
switch(side)
{
  case 0:Ymax+=step;Ymin+=step;break;
  case 1:Xmax+=step;Xmin+=step;break;
  case 2:Ymax-=step;Ymin-=step;break;
  case 3:Xmax-=step;Xmin-=step;break;break;
}

//printf("Ymax=%Lf Ymin=%Lf Xmax=%Lf Xmin=%Lf\n",Ymax,Ymin,Xmax,Xmin);
}

_Bool flag=1;
void GRline()
{
  double step = (Xmax-Xmin)/(double)width;
  double stepY=(Ymax-Ymin)/(double)heigh;
  double prevY=0x0, curY;
for(double curX=Xmin;curX<=Xmax;curX+=step)
{
  curY=Tcalculate(curX, top);
  testfunc(curX,curY,0xFF,0,0,1); 
//printf("%lf %lf\n",abs(prevY-curY),abs(Ymax-Ymin));
if(prevY && fabs(prevY-curY)<fabs(Ymax-Ymin))
{
  
  if(prevY<curY)while(prevY<curY){ testfunc(curX,prevY,0xFF,0,0,1);prevY+=stepY;}
  else if(prevY>curY)while(prevY>curY){ testfunc(curX,prevY,0xFF,0,0,1);prevY-=stepY;}
}
  prevY=curY;
}
}
/*
void GRsetPix(double X,double Y,byte R, byte G, byte B, byte mod)
{
 unsigned int xpix=width/((Xmax-X)/(X-Xmin)+1);
 unsigned int ypix=heigh/((Ymax-Y)/(Y-Ymin)+1);
 unsigned int tekpix=(ypix*width+xpix)*3;
 if(ypix>=heigh || xpix>=width)return;

  colors[tekpix]=B;
  colors[tekpix+1]=G;
  colors[tekpix+2]=R;
if(mod)
  {
  if((ypix>1) && ypix<(heigh-1))
    {
      colors[tekpix+width*3+2]=R;
      colors[tekpix+width*3+1]=G;
      colors[tekpix+width*3]  =B;

      colors[tekpix-width*3+2]=R;
      colors[tekpix-width*3+1]=G;
      colors[tekpix-width*3]  =B;
    }
  if((xpix>1) && xpix<(width-1))
    {
      colors[tekpix+3+2]=R;
      colors[tekpix+3+1]=G;
      colors[tekpix+3]  =B;

      colors[tekpix-3+2]=R;
      colors[tekpix-3+1]=G;
      colors[tekpix-3]  =B;
    }
  }
  else;
}*/