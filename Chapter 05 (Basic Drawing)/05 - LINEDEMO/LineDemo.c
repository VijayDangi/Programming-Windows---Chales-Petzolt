/*
 * Line-Drawing Demonstration Program
 */
 
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR       szAppName[] = TEXT("Line Demo");
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
		MessageBox(NULL, TEXT("Error while registering  class."), szAppName, MB_ICONERROR);
		return(0);
	}
	
	hWnd = CreateWindow(szAppName,
						TEXT("Line Demo"),
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int  cxClient, cyClient;
	HDC         hdc;
	PAINTSTRUCT ps;
	
	switch(message)
	{
		case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			return(0);
		}
		
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			
			Rectangle(
					hdc,
					cxClient / 8,
					cyClient / 8,
					7 * cxClient / 8,
					7 * cyClient / 8
				);
			
			MoveToEx (hdc, 0       , 0       , NULL);
			LineTo   (hdc, cxClient, cyClient);
			
			MoveToEx(hdc, cxClient, 0       , NULL);
			LineTo  (hdc, 0       , cyClient);
			
			Ellipse(
				hdc,
				cxClient / 8,
				cyClient / 8,
				7 * cxClient / 8,
				7 * cyClient / 8
			);
			
			RoundRect(
				hdc,
				cxClient / 4,
				cyClient / 4,
				3 * cxClient / 4,
				3 * cyClient / 4,
				cxClient / 4,
				cyClient / 4
			);
			
			EndPaint(hWnd, &ps);
			
			return(0);
		}
		
		case WM_DESTROY:
			PostQuitMessage(0);
			return(0);
	}
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
