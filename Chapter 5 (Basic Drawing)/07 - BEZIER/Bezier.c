/*
 *  Bezier Splines
 */
 
#include <windows.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR      szAppName[] = TEXT("BezierSpline");
	HWND       hWnd;
	MSG        msg;
	WNDCLASSEX wndclass;
	
	wndclass.cbSize         = sizeof(WNDCLASSEX);
	wndclass.style          = CS_VREDRAW | CS_HREDRAW;
	wndclass.lpfnWndProc    = WndProc;
	wndclass.lpszClassName  = szAppName;
	wndclass.lpszMenuName   = NULL;
	wndclass.cbClsExtra     = 0;
	wndclass.cbWndExtra     = 0;
	wndclass.hIcon          = LoadIcon(NULL, IDI_QUESTION);
	wndclass.hIconSm        = LoadIcon(NULL, IDI_ASTERISK);
	wndclass.hCursor        = LoadCursor(NULL, IDC_CROSS);
	wndclass.hInstance      = hInstance;
	wndclass.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR);
		return(0);
	}
	
	hWnd = CreateWindow(szAppName,
						TEXT("Bezier Curve Demo"),
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
	
	return (msg.wParam);
}

void DrawBezier(HDC hdc, POINT apt[])
{
	PolyBezier(hdc, apt, 4	);
	// POINT st[] = { apt[0].x, apt[0].y,
				   // apt[1].x, apt[1].y,
				   // apt[2].x, apt[2].y,
				   // apt[3].x, apt[3].y,
				   // 100, 100,
				   // 200, 200,
				   // 320, 320};
	
	// PolyBezier(hdc, st, 7);
	
	MoveToEx(hdc, apt[0].x, apt[0].y, NULL);
	LineTo(hdc, apt[1].x, apt[1].y);
	
	MoveToEx(hdc, apt[2].x, apt[2].y, NULL);
	LineTo(hdc, apt[3].x, apt[3].y);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static POINT  apt[4];
	HDC           hdc;
	int           cxClient, cyClient;
	PAINTSTRUCT   ps;
	
	switch(message)
	{
		case WM_SIZE:
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			
			apt[0].x = cxClient / 4;
			apt[0].y = cyClient / 2;
			
			apt[1].x = cxClient / 2;
			apt[1].y = cyClient / 4;
			
			apt[2].x =     cxClient / 2;
			apt[2].y = 3 * cyClient / 4;
			
			apt[3].x = 3 * cxClient / 4;
			apt[3].y =     cyClient / 2;
			
			return(0);
			
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		
			if(wParam & MK_LBUTTON || wParam & MK_RBUTTON)
			{
				hdc = GetDC(hWnd);
				//SelectObject(hdc, GetStockObject(WHITE_PEN));
				SelectObject(hdc, GetStockObject(WHITE_PEN));
				DrawBezier(hdc, apt);
				
				if(wParam & MK_LBUTTON)
				{
					apt[1].x = LOWORD(lParam);
					apt[1].y = HIWORD(lParam);
				}
				
				if(wParam & MK_RBUTTON)
				{
					apt[2].x = LOWORD(lParam);
					apt[2].y = HIWORD(lParam);
				}
				
				SelectObject(hdc, GetStockObject(BLACK_PEN));
				DrawBezier(hdc, apt);
				ReleaseDC(hWnd, hdc);
			}
			
			return(0);
			
		case WM_MOUSEMOVE:
			if(wParam & MK_LBUTTON || wParam & MK_RBUTTON)
			{
				hdc = GetDC(hWnd);
				//SelectObject(hdc, GetStockObject(WHITE_PEN));
				SelectObject(hdc, GetStockObject(WHITE_PEN));
				DrawBezier(hdc, apt);
				
				if(wParam & MK_LBUTTON)
				{
					apt[1].x = LOWORD(lParam);
					apt[1].y = HIWORD(lParam);
				}
				
				if(wParam & MK_RBUTTON)
				{
					apt[2].x = LOWORD(lParam);
					apt[2].y = HIWORD(lParam);
				}
				
				SelectObject(hdc, GetStockObject(BLACK_PEN));
				DrawBezier(hdc, apt);
				ReleaseDC(hWnd, hdc);
			}
			return(0);
			
		case WM_PAINT:
			InvalidateRect(hWnd, NULL, TRUE);
			
			hdc = BeginPaint(hWnd, &ps);
			
			DrawBezier(hdc, apt);
			
			EndPaint(hWnd, &ps);
			
			return(0);
			
		case WM_DESTROY:
			PostQuitMessage(0);
			return(0);

	}
	
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
