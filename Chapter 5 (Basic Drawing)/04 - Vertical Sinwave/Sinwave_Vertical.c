/*
 * Sine Wave using Polyline
 */
#include <windows.h>
#include <math.h>

#define NUM   1000
#define TWOPI (2 * 3.14159)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR      szAppName[] = TEXT("SinWave");
	HWND       hWnd;
	MSG        msg;
	WNDCLASSEX wndclass;
	
	wndclass.cbSize        = sizeof(WNDCLASSEX);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName  = szAppName;
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
						TEXT("Sine Wave"),
						WS_OVERLAPPEDWINDOW,
						0,
						0,
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
	int         i;
	PAINTSTRUCT ps;
	POINT       apt[NUM];
	
	switch(message)
	{
		case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			break;
		}
		
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			
			//divide line
			MoveToEx(hdc, 0       , cyClient / 2, NULL);
			LineTo  (hdc, cxClient, cyClient / 2);
			
			for(i = 0; i < NUM; i++)
			{
                apt[i].y = i * cyClient / NUM ;
				apt[i].x = (int) (cxClient / 2 * (1 - sin(TWOPI * i / NUM)));
			}
			
			Polyline(hdc, apt, NUM);
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
