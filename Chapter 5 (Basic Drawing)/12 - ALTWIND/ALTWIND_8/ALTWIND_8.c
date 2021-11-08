#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR iCmdLine, int iCmdShow)
{
	TCHAR      szAppName[] = TEXT("ALTWIND");
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
	wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR);
		return(0);
	}
	
	hWnd = CreateWindow(szAppName,
						TEXT("ALTERNATE AND WINDING Version 6 - LOGBRUSH"),
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
	static POINT aptFigure[10] = {
		10,70,  50,70,  50,10,  90,10,  90,50,
		30,50,  30,90,  70,90,  70,30,  10,30
	};
	
	static int   cxClient, cyClient;
	HDC          hdc;
	int          i;
	PAINTSTRUCT  ps;
	POINT        apt[10];
	HGDIOBJ      oldObj;
	HBRUSH       hBrush;
	TCHAR        str[100];
	TEXTMETRIC   tm;
	LOGBRUSH     logBrush;
	
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
			
			GetTextMetrics(hdc, &tm);
			oldObj = SelectObject(hdc, GetStockObject(DC_PEN));
			SetDCPenColor(hdc, RGB(255, 255, 255));
			SetTextColor(hdc, RGB(255, 0, 0));
			
			for(i = 0; i < 10; i++)
			{
				apt[i].x = cxClient * aptFigure[i].x / 200;
				apt[i].y = cyClient * aptFigure[i].y / 100;
			}
			
			/*
			 * lbStyle(UINT)             lbColor(COLORREF)       lbHatch(LONG)
			 *--------------------------------------------------------------------
			 * BS_SOLID                   Color of brush          ignored
			 * BS_HOLLOW                  ignored                 ignored
			 * BS_HATCHED                 Color of Hatches        Hatch brush style
			 * BS_PATTERN                 ignored                 Handle to bitmap
			 * BS_DIBPATTERNPT            ignored                 Pointer to DIB
			 */
			logBrush.lbStyle = BS_HATCHED;
			logBrush.lbColor = RGB(255, 0, 255);
			logBrush.lbHatch = HS_BDIAGONAL ;
			SelectObject(hdc, CreateBrushIndirect(&logBrush));
			
			SetBkMode(hdc, OPAQUE);
			SetPolyFillMode(hdc, ALTERNATE);
			Polygon(hdc, apt, 10);
			
			TextOut(hdc, apt[0].x, 0                                       , str, wsprintf(str, TEXT("Background Mode   : OPAQUE")));
			TextOut(hdc, apt[0].x,      tm.tmHeight + tm.tmExternalLeading , str, wsprintf(str, TEXT("PolygonFillMode   : ALTERNATE")));
			TextOut(hdc, apt[0].x, 2 * (tm.tmHeight + tm.tmExternalLeading), str, wsprintf(str, TEXT("Style   : HS_BDIAGONAL")));
						
			for(i = 0; i < 10; i++)
			{
				apt[i].x += cxClient / 2;
			}
			
			
			SetBkMode(hdc, TRANSPARENT);
			SetPolyFillMode(hdc, WINDING);
			Polygon(hdc, apt, 10);
			
			TextOut(hdc, apt[0].x, 0                                       , str, wsprintf(str, TEXT("Background Mode   : TRANSPARENT")));
			TextOut(hdc, apt[0].x,      tm.tmHeight + tm.tmExternalLeading , str, wsprintf(str, TEXT("PolygonFillMode   : WINDING")));
			TextOut(hdc, apt[0].x, 2 * (tm.tmHeight + tm.tmExternalLeading), str, wsprintf(str, TEXT("Style   : HS_BDIAGONAL")));
            
			DeleteObject(SelectObject(hdc, oldObj));
			EndPaint(hWnd, &ps);
			
			return(0);
		}
		
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
	}
	return(DefWindowProc(hWnd, message, wParam, lParam));
}

