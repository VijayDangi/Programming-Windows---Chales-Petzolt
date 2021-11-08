#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR      szAppName[] = TEXT("BRUSH");
	HWND       hWnd;
	MSG        msg;
	WNDCLASSEX wndclass;
	
	wndclass.cbSize        = sizeof(WNDCLASSEX);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName  = NULL;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hInstance     = hInstance;
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpfnWndProc   = WndProc;
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR);
		return(0);
	}
	
	hWnd = CreateWindow(szAppName,
						TEXT("Brush Demo"),
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
	
	static int  cxClient, cyClient;
	HDC         hdc;
	int         i;
	PAINTSTRUCT ps;
	POINT        apt[10];
	HGDIOBJ     oldObj;
	HBRUSH      hBrush;
	
	
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
			
			oldObj = SelectObject(hdc, CreateSolidBrush(RGB(125, 200, 87)));
			
			for(i = 0; i < 10; i++)
			{
				apt[i].x = cxClient * aptFigure[i].x / 200;				
				apt[i].y = cyClient * aptFigure[i].y / 100;				
			}
			SetPolyFillMode(hdc, ALTERNATE);
			Polygon(hdc, apt, 10);
			
			for(i = 0; i < 10; i++)
			{
				apt[i].x += cxClient / 2;
			}
			SetPolyFillMode(hdc, WINDING);
			Polygon(hdc, apt, 10);
			
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
