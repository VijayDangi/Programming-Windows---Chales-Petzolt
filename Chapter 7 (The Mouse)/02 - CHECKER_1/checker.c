/*
 * Mouse Hit-Test Demo Program #1
 */

#include <windows.h>

#define DIVISIONS 5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR       szAppName[] = TEXT("CHECKER_1");
	HWND        hWnd;
	MSG         msg;
	WNDCLASSEX  wndclass;
	
	wndclass.cbSize        = sizeof(WNDCLASSEX);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName  = NULL;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hIcon         = LoadIcon(NULL, IDC_ARROW);
	wndclass.hIconSm       = LoadIcon(NULL, IDC_ARROW);
	wndclass.hInstance     = hInstance;
	wndclass.hCursor       = LoadCursor(NULL, IDI_APPLICATION);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registerig class."), szAppName, MB_OK);
		return(0);
	}
	
	hWnd = CreateWindow(
				szAppName,
				TEXT("Checker_1 Mouse Hit-Test Demo"),
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
			
	if(!hWnd)
	{
		MessageBox(NULL, TEXT("Error while creating window."), szAppName, MB_OK);
		return(0);
	}
	
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL  fState[DIVISIONS][DIVISIONS];
	static int   cxBlock, cyBlock;
	HDC          hDC;
	int          x, y;
	PAINTSTRUCT  ps;
	RECT         rc;
	
	switch(message)
	{
		case WM_SIZE:
			cxBlock = LOWORD(lParam) / DIVISIONS;
			cyBlock = HIWORD(lParam) / DIVISIONS;
			return(0);
			
		case WM_LBUTTONDOWN:
			x = LOWORD(lParam) / cxBlock;
			y = HIWORD(lParam) / cyBlock;
			
			if(x < DIVISIONS && y < DIVISIONS)
			{
				fState [x][y] ^= 1;
				
				rc.left   = x * cxBlock;
				rc.top    = y * cyBlock;
				rc.right  = (x + 1) * cxBlock;
				rc.bottom = (y + 1) * cyBlock;
				
				InvalidateRect(hWnd, &rc, FALSE);
			}
			else
				MessageBeep(0);
			
			return(0);
			
		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			
			for(x = 0; x < DIVISIONS; x++)
				for(y = 0; y < DIVISIONS; y++)
				{
					Rectangle(hDC, x * cxBlock, y * cyBlock,
									(x + 1) * cxBlock, (y + 1) * cyBlock);
									
					if(fState[x][y])
					{
						MoveToEx(hDC,  x      * cxBlock,  y      * cyBlock, NULL);
						LineTo  (hDC, (x + 1) * cxBlock, (y + 1) * cyBlock);
						MoveToEx(hDC,  x      * cxBlock, (y + 1) * cyBlock, NULL);
						LineTo  (hDC, (x + 1) * cxBlock,  y      * cyBlock);
						
					}
				}
			
			EndPaint(hWnd, &ps);
			
			return(0);
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
