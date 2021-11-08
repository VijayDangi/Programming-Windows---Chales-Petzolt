/*
 * Clover Drawing Program Using Regions
 */

#include <windows.h>
#include <math.h>
#include <stdio.h>

#define TWO_PI (2.0 * 3.14159)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR      szAppName[] = TEXT("Clover");
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
	static HRGN   hRgnClip;
	static int    cxClient, cyClient;
	static double fAngle, fRadius = 581.00;
	HCURSOR       hCursor;
	HDC           hdc;
	HRGN          hRgnTemp[6];
	int           i;
	PAINTSTRUCT   ps;
	
	switch(message)
	{
		case WM_CREATE:
		{
			break;
		}
		
		case WM_TIMER:
		{
			KillTimer(hWnd, 100);
			InvalidateRgn(hWnd, hRgnClip, FALSE);
			// InvalidateRect(hWnd, NULL, FALSE);
			SetTimer(hWnd, 100, 500, NULL);	
			break;
		}
		
		case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			
			hCursor = SetCursor(LoadCursor(NULL, IDC_WAIT));
			ShowCursor(TRUE);
			
			if(hRgnClip)
				DeleteObject(hRgnClip);
			
			hRgnTemp[0] = CreateEllipticRgn(0, cyClient / 3, cxClient / 2,	2 * cyClient / 3);
			
			hRgnTemp[1] = CreateEllipticRgn(cxClient / 2, cyClient / 3, cxClient, 2 * cyClient / 3);
			
			hRgnTemp[2] = CreateEllipticRgn(cxClient / 3, 0, 2 * cxClient / 3, cyClient / 2);
			
			hRgnTemp[3] = CreateEllipticRgn(cxClient / 3, cyClient / 2, 2 * cxClient / 3, cyClient);
			
			hRgnTemp[4] = CreateEllipticRgn(0, 0, 1, 1);
			
			hRgnTemp[5] = CreateEllipticRgn(0, 0, 1, 1);
			
			hRgnClip    = CreateEllipticRgn(0, 0, 1, 1);
			
			CombineRgn(hRgnTemp[4], hRgnTemp[0], hRgnTemp[1], RGN_OR);
			CombineRgn(hRgnTemp[5], hRgnTemp[2], hRgnTemp[3], RGN_OR);
			CombineRgn(hRgnClip   , hRgnTemp[4], hRgnTemp[5], RGN_XOR);
			
			for(i = 0; i < 6; i++)
				DeleteObject(hRgnTemp[i]);
			
			fRadius = _hypot(cxClient/2.0, cyClient/2.0);
			
			SetCursor(hCursor);
			ShowCursor(FALSE);
			return(0);
		}
		
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			
			SetWindowOrgEx(hdc, -cxClient/2, -cyClient/2, NULL);
			// SelectClipRgn(hdc, hRgnClip);
			
			
				MoveToEx(hdc, 0, 0, NULL);
				LineTo(hdc, (int) ( fRadius * cos(fAngle) + 0.5),
							(int) (-fRadius * sin(fAngle) + 0.5));

			
			if(fAngle <= TWO_PI)
			{
				fAngle = fAngle + TWO_PI / 360;
				SetTimer(hWnd, 100,500, NULL);
			}
			
			EndPaint(hWnd, &ps);
			
			break;
		}
		
		case WM_DESTROY:
		{
			DeleteObject(hRgnClip);
			PostQuitMessage(0);
			return(0);
		}
	}
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
