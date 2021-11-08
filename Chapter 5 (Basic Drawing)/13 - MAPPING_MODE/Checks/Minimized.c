
#include <windows.h>
 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR      szAppName[] = TEXT("WhatSize");
	HWND       hWnd;
	MSG        msg;
	WNDCLASSEX wndclass;
	
	wndclass.cbSize        = sizeof(WNDCLASSEX);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName  = NULL;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hInstance     = hInstance;
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR);
		return(0);
	}
	
	hWnd = CreateWindow(szAppName,
						TEXT("What Size is the window?"),
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
					
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return(msg.wParam);
}

void Show(HWND hWnd, HDC hdc, int xText, int yText, int iMapMode, TCHAR *szMapMode)
{
	TCHAR szBuffer[60];
	RECT  rect;
	
	SaveDC(hdc);    //save current device context attribute
	
	SetMapMode(hdc, iMapMode);
	GetClientRect(hWnd, &rect);
	DPtoLP(hdc, (PPOINT) &rect, 2);
	
	RestoreDC(hdc, -1);   //restore latest saved device context
	
	TextOut(hdc, xText, yText, szBuffer,
				wsprintf(szBuffer, TEXT("%-20s %7d %7d %7d %7d"), szMapMode, rect.left, rect.right, rect.top, rect.bottom));
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hbrWhite, hbrGray;
	static int    total;
	static int    enableMappingMode = 0;
	HDC           hdc;
	PAINTSTRUCT   ps;
	RECT          rc;
	int           i, x, y;
	static POINT  aptStar[6] = { 50,2,  2,98,  98,33,  2,33,  98,98,  50,2};
	
	switch(message)
	{
		case WM_CREATE:
		{
			hbrWhite = (HBRUSH) GetStockObject(WHITE_BRUSH);
			hbrGray  = (HBRUSH) GetStockObject(GRAY_BRUSH);
		}
		
		case WM_CHAR:
		{
			if(wParam == 'i'  || wParam == 'I')
			{
				if(total < 13)
				{
					total++;
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			else if(wParam == 'd' || wParam == 'D')
			{
				if(total > 0)
				{
					total--;
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
			else if(wParam == 'e' || wParam == 'E')
			{
				enableMappingMode = !enableMappingMode;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			
			break;
		}
		
		case WM_ERASEBKGND:
		{
			hdc = (HDC)wParam;
			GetClientRect(hWnd, &rc);
			
			if(enableMappingMode)
			{
				SetMapMode(hdc, MM_ANISOTROPIC);
				SetWindowExtEx(hdc, 100, 100, NULL);
				SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
			}
			
			FillRect(hdc, &rc, hbrGray);
			
			for(i = 0; i < total; i++)
			{
				x = (i * 40) % 100;
				y = ((i * 40) / 100) * 20;
				SetRect(&rc, x, y, x + 20, y + 20);
				FillRect(hdc, &rc, hbrWhite);
			}
			
			ReleaseDC(hWnd, hdc);
			
			return (1L);
		}
		
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			
			if(!IsIconic(hWnd))
			{
				GetClientRect(hWnd, &rc);
				SetMapMode(hdc, MM_ANISOTROPIC);
				SetWindowExtEx(hdc, 100, 100, NULL);
				SetViewportExtEx(hdc, rc.right, rc.bottom, NULL);
				
				Polyline(hdc, aptStar, 6);
			}
			else
			{
				TextOut(hdc, 0, 0, "Hello", 5);
			}
			
			EndPaint(hWnd, &ps);
			return (0L);
		}
		
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return(0);
		}
	}
	
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
