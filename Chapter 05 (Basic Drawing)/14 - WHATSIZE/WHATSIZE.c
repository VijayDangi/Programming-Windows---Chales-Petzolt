/*
 *  What Size is the window?
 */
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
						TEXT("What Size is the window? x = cxChar, y = cyChar"),
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
	static TCHAR szHeading[] = TEXT("Mapping Mode            Left   Right     Top  Bottom");
	static TCHAR szUndLine[] = TEXT("------------            ----   -----     ---  -------");
	static int   cxChar, cyChar;
	HDC          hdc;
	PAINTSTRUCT  ps;
	TEXTMETRIC   tm;
	switch(message)
	{
		case WM_CREATE:
		{
			hdc = GetDC(hWnd);
			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
			
			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			cyChar = tm.tmHeight + tm.tmExternalLeading;
			
			ReleaseDC(hWnd, hdc);
			return(0);
		}

		
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
			
			SetMapMode(hdc, MM_ANISOTROPIC);
			SetWindowExtEx(hdc, 1, 1, NULL);
			SetViewportExtEx(hdc, cxChar, cyChar, NULL);
			
			TextOut(hdc, 1, 1, szHeading, lstrlen(szHeading));
			TextOut(hdc, 1, 2, szUndLine, lstrlen(szUndLine));
			
			Show(hWnd, hdc, 1, 3, MM_TEXT     , TEXT("TEXT(pixels)")        );
			Show(hWnd, hdc, 1, 4, MM_LOMETRIC , TEXT("LOMETRIC (0.1 mm)")   );
			Show(hWnd, hdc, 1, 5, MM_HIMETRIC , TEXT("HIMETRIC (0.01 mm)")  );
			Show(hWnd, hdc, 1, 6, MM_LOENGLISH, TEXT("LOENGLISH (0.01 in)") );
			Show(hWnd, hdc, 1, 7, MM_HIENGLISH, TEXT("HIENGLISH (0.001 in)"));
			Show(hWnd, hdc, 1, 8, MM_TWIPS    , TEXT("TWIPS (1/1440 in)")   );
			
			EndPaint(hWnd, &ps);
			return (0);
		}
		
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return(0);
		}
	}
	
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
