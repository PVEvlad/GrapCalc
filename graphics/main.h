#include <stdio.h>
#include <windows.h>
#include <commctrl.h>

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow);
LRESULT CALLBACK SubEditProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam); 
DWORD WINAPI KeyProc(LPVOID lpParameter);
void DrawCoord(HWND);