#include <windows.h>
#include "Resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR iCmdLine, int iCmdShow)
{
	//local variables
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MATH_COLOR");


	//code

	//initialize wndclass
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszClassName = szAppName;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszMenuName = NULL;

	//register above class
	RegisterClassEx(&wndclass);

	//create window in memory
	hwnd = CreateWindow(szAppName,
		TEXT("INVERT RECT"),
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

	//show window
	ShowWindow(hwnd, SW_MAXIMIZE);//SW_MAXIMIZE);
	UpdateWindow(hwnd);

	//message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return ((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBITMAP   hBitmap;
	static HDC       hdc, hdcMem;
	PAINTSTRUCT      ps;
	static BITMAP    bitmap;
	static HINSTANCE hInstance;
	static int       flag  = 0;
	static int       cxClient, cyClient;
	RECT rc;
	static int bit = 1;
	
	switch(message)
	{
		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			break;
			
		case WM_CREATE:
			hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
			break;
			
		case WM_CHAR:
			
			switch(wParam)
				{
					case '1':
						bit = 1;
						break;
						
					case '2':
						bit = 2;
						break;
						
					case '3':
						bit = 3;
						break;
						
					case '4':
						bit = 4;
						break;
						
					case '5':
						bit = 5;
						break;
						
					case '6':
						bit = 6;
						break;
				}
			
			if(wParam == 'l' || wParam == 'L')
			{
				// MessageBox(hWnd, TEXT("L"), TEXT("L"), MB_OK);
				flag = 1;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if(wParam == 'i')
			{
				// MessageBox(hWnd, TEXT("x++"), TEXT(""), MB_OK);
				flag = 2;
				InvalidateRect(hWnd, NULL, FALSE);
			}
			else
			{
				flag = 0;
				// MessageBox(hWnd, TEXT("na"), TEXT(""), MB_OK);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			
			break;
			
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			
			if(flag == 1)
			{
				if(hBitmap)
					DeleteObject(hBitmap);
			
				
				GetClientRect(hWnd, &rc);
				
				switch(bit)
				{
					case 1:
						hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(MYBITMAP));
						break;
						
					case 2:
						hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(BLACKHOLE));
						break;
						
					case 3:
						hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(PATTERN));
						break;
						
					case 4:
						hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(INDIA));
						break;
						
					case 5:
						hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(SPHERE));
						break;
						
					case 6:
						hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(WAVE));
						break;
				}		
				
				hdcMem = CreateCompatibleDC(NULL);
				SelectObject(hdcMem, hBitmap);
				GetObject(hBitmap, sizeof(BITMAP), (PSTR)&bitmap);
				
				StretchBlt(   hdc, 0, 0, rc.right, rc.bottom,
						   hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
				// StretchBlt(   hdc, 0, 0, rc.right, rc.bottom,
						   // hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
						   
				// DeleteObject(hdcMem);
				// DeleteObject(hBitmap);
			}
			else if(flag == 2)
			{
				if(!hBitmap)
					break;
				
				GetClientRect(hWnd, &rc);
				InvertRect(hdc, &rc);
				
			}
			EndPaint(hWnd, &ps);
			
			break;
		}
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
