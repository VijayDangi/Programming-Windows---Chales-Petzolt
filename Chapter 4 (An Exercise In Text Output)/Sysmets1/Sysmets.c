/**
 * System Metrics Display
 **/
 
#include <windows.h>
#include "Sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SysMets1");
	HWND         hwnd;
	MSG          msg;
	WNDCLASSEX   wndclass;
	
	wndclass.cbSize        = sizeof(wndclass);
	wndclass.lpfnWndProc   = WndProc;
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName  = NULL;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.hInstance     = hInstance;
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registering class"), szAppName, MB_ICONERROR);
		return(0);
	}
	
	hwnd = CreateWindow(szAppName,
						szAppName,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						NULL,
						NULL,
						hInstance,
						NULL
					);
					
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int  cxChar, cxCaps, cyChar;
	HDC         hdc;
	int         i;
	PAINTSTRUCT ps;
	TCHAR       szBuffer[10];
	TEXTMETRIC  tm;
	
	switch(message)
	{
		case WM_CREATE:
		{
			hdc = GetDC(hWnd);
			
			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			/* 
				The low bit of the tmPitchAndFamily field in the TEXTMETRIC structure is 1 for a variable-width font,
				and 0 for a fixed-pitch font. this program uses this bit to calculate cxCaps from cxChar.
			 */
			cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
			cyChar = tm.tmHeight + tm.tmExternalLeading;
			
			ReleaseDC(hWnd, hdc);
			return(0);
		}
		
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			
			for( i = 0; i < NUMLINES; i++)
			{
				TextOut(hdc,
						0,
						cyChar * i,
						sysmetrics[i].szLabel,
						lstrlen(sysmetrics[i].szLabel)
					);
				
				TextOut(hdc,
						22 * cxCaps,
						cyChar * i,
						sysmetrics[i].szDesc,
						lstrlen(sysmetrics[i].szDesc)
					);
				
				SetTextAlign(hdc, TA_RIGHT | TA_TOP);
				
				TextOut(hdc,
						22 * cxCaps + 40 * cxChar,
						cyChar * i,
						szBuffer,
						wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex))
					);
					
				SetTextAlign(hdc, TA_LEFT | TA_TOP);
			}
			
			EndPaint(hWnd, &ps);
			return(0);
		}
		
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return (0);
		}
	}
	
	return (DefWindowProc(hWnd, message, wParam, lParam));
}
