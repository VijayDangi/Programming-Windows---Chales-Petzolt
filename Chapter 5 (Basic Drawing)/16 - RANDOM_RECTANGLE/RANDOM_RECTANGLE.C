/*
 * Display Random Rectangles
 */

#include <windows.h>
#include <stdlib.h>   //for rand function

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawRectangle(HWND);

int cxClient, cyClient;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR       szAppName[] = TEXT("RandRect");
	HWND        hWnd;
	MSG         msg;
	WNDCLASSEX  wndclass;
	
	wndclass.cbSize          = sizeof(WNDCLASSEX);
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
		MessageBox(NULL, TEXT("Error while registering class."), TEXT(""), MB_ICONERROR);
		return(0);
	}
	
	hWnd = CreateWindow(szAppName,
						TEXT("Random Rectangles"),
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
	
	while(TRUE)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			DrawRectangle(hWnd);
	}
	return(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			return(0);
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return(0);
	}
	return(DefWindowProc(hWnd, message, wParam, lParam));
}

void DrawRectangle(HWND hWnd)
{
	HBRUSH hBrush;
	HDC    hdc;
	RECT   rect;
	
	if(cxClient == 0 || cyClient == 0)
		return;
	
	SetRect(&rect, rand() % cxClient, rand() % cyClient,
				   rand() % cxClient, rand() % cyClient );
				   
	hBrush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
	
	hdc = GetDC(hWnd);
	
	FillRect(hdc, &rect, hBrush);
	ReleaseDC(hWnd, hdc);
	DeleteObject(hBrush);
}
