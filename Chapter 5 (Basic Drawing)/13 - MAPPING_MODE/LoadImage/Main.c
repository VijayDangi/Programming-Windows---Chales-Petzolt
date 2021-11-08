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
		TEXT("MATHEMATICS AND COLOR MAGIC"),
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
	static int       x, y;
	RECT rc;
	
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
			if(wParam == 'l' || wParam == 'L')
			{
				// MessageBox(hWnd, TEXT("L"), TEXT("L"), MB_OK);
				flag = 1;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if(wParam == 'X')
			{
				x+=10;
				// MessageBox(hWnd, TEXT("x++"), TEXT(""), MB_OK);
				flag = 2;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else if(wParam == 'x')
			{
				x-=10;
				// MessageBox(hWnd, TEXT("x--"), TEXT(""), MB_OK);
				flag = 2;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			
			else if(wParam == 'Y')
			{
				y+=10;
				// MessageBox(hWnd, TEXT("y++"), TEXT(""), MB_OK);
				flag = 2;
				InvalidateRect(hWnd, NULL, TRUE);
			}
			
			else if(wParam == 'y')
			{
				y-=10;
				// MessageBox(hWnd, TEXT("y--"), TEXT(""), MB_OK);
				flag = 2;
				InvalidateRect(hWnd, NULL, TRUE);
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
			RECT rc;
			hdc = BeginPaint(hWnd, &ps);
			
			
			
			if(flag == 1)
			{
				if(hBitmap)
					DeleteObject(hBitmap);
			
				
				GetClientRect(hWnd, &rc);
				hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(MYBITMAP));
				hdcMem = CreateCompatibleDC(NULL);
				SelectObject(hdcMem, hBitmap);
				GetObject(hBitmap, sizeof(BITMAP), (PSTR)&bitmap);
				
				SetMapMode(hdcMem, MM_ANISOTROPIC);
				SetWindowExtEx(hdcMem, 1920, 1080, NULL);
				SetViewportExtEx(hdcMem, cxClient/2, -cyClient/2, NULL);
				SetViewportOrgEx(hdcMem, cxClient/2, cyClient/2, NULL);
				
				StretchBlt(   hdc, 0, 0, rc.right, rc.bottom,
						   hdcMem, 0, 0, -bitmap.bmWidth, -bitmap.bmHeight, SRCCOPY);
				// StretchBlt(   hdc, 0, 0, rc.right, rc.bottom,
						   // hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
						   
				// DeleteObject(hdcMem);
				// DeleteObject(hBitmap);
			}
			else if(flag == 2)
			{
				GetClientRect(hWnd, &rc);
				
				// SetMapMode(hdcMem, MM_ANISOTROPIC);
				// SetWindowExtEx(hdcMem, 1920, 1080, NULL);
				// SetViewportExtEx(hdcMem, cxClient/2, -cyClient/2, NULL);
				// SetViewportOrgEx(hdcMem, cxClient/2, cyClient/2, NULL);
				StretchBlt(   hdc, 0, 0, rc.right, rc.bottom,
						   hdcMem, x , y,  -bitmap.bmWidth , -bitmap.bmHeight , SRCCOPY);
						   
			}
			
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 10, 0, TEXT("x : Right"), 9);
			TextOut(hdc, 10, 25, TEXT("X : Left") , 8);
			TextOut(hdc, 10, 50, TEXT("y : down") , 8);
			TextOut(hdc, 10, 75, TEXT("Y : Up")   , 6);
			EndPaint(hWnd, &ps);
			
			break;
		}
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
