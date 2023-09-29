#include "main.h"

WNDCLASS mywin={0,WindowProc,0,0,0,0,0,0,0,"mywin"};
HDC hdc;
HWND window, functionEdit, coord;
HFONT font;
HPEN pen;
HCURSOR curs;
BYTE keyboard[256];
char function[512];
extern int width,heigh;
extern int error;
extern double Xmin,Ymin,Xmax,Ymax;
extern void GRmove(byte);

LRESULT (*oldEdit)(HWND,UINT,WPARAM,LPARAM);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
FreeConsole();
GRinit();
memset(function,'\0',512);
mywin.hInstance=hInstance;
mywin.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);


if(!RegisterClass(&mywin))MessageBox(0,"NoClass","",0);
window=CreateWindow("mywin","WindowB",WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,0,0,width,heigh,0,0,hInstance,0);

if(!window) MessageBox(0,"NoWindow","",0);

functionEdit=CreateWindow("EDIT","sqrt(1-x^2)",WS_BORDER | WS_CHILD,0,0,width,heigh/20,window,0,hInstance,0);
coord=CreateWindow("STATIC","x;y",WS_CHILD,width*(2.0/3.0),heigh/20,width/3,heigh/20,window,0,hInstance,0);

font=CreateFont(heigh/20-2, heigh/40,0,0,4,0,0,0,0,0,0,0,5,0);
SendMessage(coord, WM_SETFONT, font, 0);
SendMessage(functionEdit, WM_SETFONT, font, 0);

ShowWindow(coord,5);
ShowWindow(functionEdit,5);
ShowWindow(window,5);
SetTimer(window,0x0100,15,KeyProc);

oldEdit=SetWindowLongPtr(functionEdit,GWLP_WNDPROC,SubEditProc);
//SetWindowSubclass(functionEdit,SubEditProc,0x1524,0);
curs=LoadCursor(0, IDC_ARROW);
MSG msg;
BOOL bRet;
SetCursor(curs);

TClear();
Tparse("sqrt(1-x^2)\0");
Tsort();
TinitTree();

while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
{ 
    if (bRet == -1)
        PostQuitMessage(0);
    else
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
}
return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
PAINTSTRUCT ps;
int par=0;
//
 switch (uMsg)
    { 
     case WM_LBUTTONDOWN: SetFocus(window);SetCursor(curs);break;
     case WM_CREATE:GRscale(0);GRscale(1); break;
     case WM_PAINT:     
    hdc=BeginPaint(window, &ps);
    GRfillMap();
    if(!error)GRline();
    par=GRdraw();
    EndPaint(window,&ps);
    DrawCoord(coord);
    break;
        case WM_MOUSEWHEEL:
            if(((signed short)HIWORD(wParam))>0) GRscale(0);
            else GRscale(1);
            InvalidateRect(window,0,0);
             break;
        case WM_DESTROY:DeleteObject(font); PostQuitMessage(0); break;
        case WM_KEYDOWN: keyboard[wParam]=1; break;
        case WM_KEYUP:   keyboard[wParam]=0; break;
   SetKeyboardState(keyboard);
        break;
	}

    return DefWindowProcA(hwnd,uMsg,wParam,lParam);
    
}

DWORD WINAPI KeyProc(LPVOID lpParameter)
{
    _Bool down, up, right, left; 
    down = keyboard[VK_DOWN], up=keyboard[VK_UP], right=keyboard[VK_RIGHT], left=keyboard[VK_LEFT];
        if(down){GRmove(2); keyboard[VK_DOWN]=1;} 
        if(up){GRmove(0);keyboard[VK_UP]=1;} 
        if(left){GRmove(3);keyboard[VK_LEFT]=1;} 
        if(right){GRmove(1);keyboard[VK_RIGHT]=1;} 
        //if(keyboard[VK_ENTER]){}
        //InvalidateRect(window,0,0);
}

LRESULT CALLBACK SubEditProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    static BYTE stop;
switch (uMsg)
{
case WM_CHAR: if(stop) {stop--;return;} break;
case WM_CREATE: break;
case WM_PAINT: break;
case WM_KEYDOWN:switch(wParam)
{
    case VK_RETURN:
    stop=1;
    memset(function,0,512);
GetWindowText(functionEdit,function,510);
TClear();
error=0; 
Tparse(function);
if(error)
{
    char str[20];
    sprintf(str,"Error %d",error);
    MessageBox(0,str,"Function Error",0);
    return oldEdit(hwnd,uMsg,wParam,lParam);
}
Tsort();
TinitTree();
SetFocus(window);
InvalidateRect(window,0,0);
    break; } break;
}
return oldEdit(hwnd,uMsg,wParam,lParam);//DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

void DrawCoord(HWND wind)
{
POINT pt;
PAINTSTRUCT sq;
HDC hdc;
char str[50];
GetCursorPos(&pt);
double X=Xmin+((double)(pt.x)/(double)width)*(Xmax-Xmin),Y=Ymax-((double)(pt.y)/(double)heigh)*(Ymax-Ymin);
InvalidateRect(coord,0,1);
memset(str,0,50);    
sprintf(str,"X=%8.3lf; Y=%8.3lf",X,Y);
hdc = BeginPaint(wind,&sq);
SelectObject(hdc,font);
TextOut(hdc,0,0,str,strlen(str));
EndPaint(wind,&sq);

return;
}