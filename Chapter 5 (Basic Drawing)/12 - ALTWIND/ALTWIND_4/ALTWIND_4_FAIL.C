#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR      szAppName[] = TEXT("BRUSH");
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
	else{
		MessageBox(NULL, TEXT("registering class."), szAppName, MB_ICONERROR);
	}
	
	hWnd = CreateWindow(szAppName,
						TEXT("ALTERNATE AND WINDING Version 4"),
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
	TCHAR str[100];
	TEXTMETRIC tm;
	static FILE *fp;
	
	switch(message)
	{
		case WM_CREATE:
			 fp = fopen("messageLog.txt", "w");
				
			break;
		case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			break;
		}
		
		// case WM_NCPAINT:
			// MessageBox(hWnd, TEXT("WM_NCPAINT"), TEXT(""), MB_OK);
			// return(0);
			
		// case WM_DISPLAYCHANGE:
			// MessageBox(hWnd, TEXT("WM_DISPLAYCHANGE"), TEXT(""), MB_OK);
			// return(0);
			
		// case WM_PRINT:
			// MessageBox(hWnd, TEXT("WM_PRINT"), TEXT(""), MB_OK);
			// return(0);
			
		// case WM_PRINTCLIENT:
			// MessageBox(hWnd, TEXT("WM_PRINTFCLIENT"), TEXT(""), MB_OK);
			// return(0);
			
		// case WM_SETREDRAW:
			// MessageBox(hWnd, TEXT("WM_SETREDRAW"), TEXT(""), MB_OK);
			// return(0);
			
		// case WM_SYNCPAINT:
			// MessageBox(hWnd, TEXT("SYNCPAINT"), TEXT(""), MB_OK);
			// return(0);
			
		// case WM_DWMNCRENDERINGCHANGED :
		// case WM_ERASEBKGND :
			// //MessageBox(hWnd, TEXT("WM_ERASEBKGND"), TEXT(""), MB_OK);
			// return(0);
		
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
			
			SetBkMode(hdc, OPAQUE);
			SelectObject(hdc, CreateHatchBrush(HS_FDIAGONAL, RGB(0, 0, 255)));
			SetPolyFillMode(hdc, ALTERNATE);
			Polygon(hdc, apt, 10);
			
			TextOut(hdc, apt[0].x, 0                                     , str, wsprintf(str, TEXT("Background Mode   : OPAQUE")));
			TextOut(hdc, apt[0].x,     tm.tmHeight + tm.tmExternalLeading, str, wsprintf(str, TEXT("PolygonFillMode   : ALTERNATE")));
			TextOut(hdc, apt[0].x, 2 * (tm.tmHeight + tm.tmExternalLeading), str, wsprintf(str, TEXT("HATCH_BRUSH style : HS_FDIAGONAL")));
			
						
			for(i = 0; i < 10; i++)
			{
				apt[i].x += cxClient / 2;
			}
			
			SetBkMode(hdc, TRANSPARENT);
			DeleteObject(SelectObject(hdc, CreateHatchBrush(HS_BDIAGONAL, RGB(0, 0, 255))));
			SetPolyFillMode(hdc, WINDING);
			Polygon(hdc, apt, 10);
			
			TextOut(hdc, apt[0].x, 0                                     , str, wsprintf(str, TEXT("Background Mode   : TRANSPARENT")));
			TextOut(hdc, apt[0].x,     tm.tmHeight + tm.tmExternalLeading, str, wsprintf(str, TEXT("PolygonFillMode   : WINDING")));
			TextOut(hdc, apt[0].x, 2 * (tm.tmHeight + tm.tmExternalLeading), str, wsprintf(str, TEXT("HATCH_BRUSH style : HS_BDIAGONAL")));
			
			DeleteObject(SelectObject(hdc, oldObj));
			EndPaint(hWnd, &ps);
			
			break;
		}
		
		case WM_DESTROY:
		{
			fclose(fp);
			PostQuitMessage(0);
			break;
		}
	}
	// LRESULT ret = ;
	if(fp != NULL)
	{
		// fprintf(fp, "%x = %d\n", message, DefWindowProc(hWnd, message, wParam, lParam));
		fprintf(fp, "%x\n", message);
	}
	// else
	// {
		// TCHAR str[10];
		// wsprintf(str, TEXT("%x"), message);
		// MessageBox(hWnd, str, TEXT(""), MB_OK);
	// }
	// return(DefWindowProc(hWnd, message, wParam, lParam));
	 
	//return(1);
}
