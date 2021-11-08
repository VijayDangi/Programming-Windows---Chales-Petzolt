/*
 * Clover Drawing Program Using Regions
 */

#include <windows.h>
#include <math.h>

#define TWO_PI (2.0 * 3.14159)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR      szAppName[] = TEXT("RGN");
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
						TEXT("RGN_OR"),
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
    static  HRGN hRgnDest;
	static  HRGN hSrcRgn1, hSrcRgn2;
	static  int  cxClient, cyClient;
    HBRUSH       hBrush;
	HDC          hdc;
	PAINTSTRUCT  ps;
	
	switch(message)
	{
		case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
						
			hSrcRgn1 = CreateEllipticRgn(0, cyClient/3, cxClient/2 + 100,	2 * cyClient/3);
			
			hSrcRgn2 = CreateEllipticRgn(cxClient/2 - 100, cyClient/3, cxClient, 2 * cyClient/3);
			
			hRgnDest = CreateEllipticRgn(0, 0, 1, 1);
            
			CombineRgn(hRgnDest  , hSrcRgn1, hSrcRgn2, RGN_OR); //***//
			
			return(0);
		}
		
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			
            hBrush = CreateSolidBrush(RGB(0, 255, 0));
			SelectObject(hdc, GetStockObject(DC_BRUSH));
            SetDCBrushColor(hdc, RGB(255, 0, 0));
            PaintRgn(hdc, hSrcRgn1);
            // FrameRgn(hdc, hSrcRgn1, hBrush, (cxClient/2) - (0), (2*cyClient/3) - (cyClient/3));
            
            FillRgn(hdc, hSrcRgn2, hBrush);
            
            SetDCBrushColor(hdc, RGB(255, 255, 0));
            PaintRgn(hdc, hRgnDest);
            
            DeleteObject(hBrush);			
			EndPaint(hWnd, &ps);
			
			return(0);
		}
		
		case WM_DESTROY:
		{
			DeleteObject(hRgnDest);
			DeleteObject(hSrcRgn1);
			DeleteObject(hSrcRgn2);
            
			PostQuitMessage(0);
			return(0);
		}
	}
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
