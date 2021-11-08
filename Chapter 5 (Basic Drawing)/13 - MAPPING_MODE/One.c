#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR      szAppName[] = TEXT("MAPPINT_MODE");
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
	wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wndclass.lpfnWndProc   = WndProc;
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR);
		return(0);
	}
	
	hWnd = CreateWindow(szAppName,
						TEXT("MAPPINT_MODE Demo"),
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
	HDC         hdc;
	PAINTSTRUCT ps;
	static int cxClient, cyClient;
	
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
			
			// Rectangle(hdc, 30, 300, 500, 315);
			
			// SetMapMode(hdc, MM_ANISOTROPIC);
			// SetWindowOrgEx(hdc, 0, 0, NULL);
			// SetWindowExtEx(hdc, 1, 1, NULL);
			
			// SetViewportOrgEx(hdc, 15, 15, NULL);
			// SetViewportExtEx(hdc, 1, 1, NULL);
			// Rectangle(hdc, 0, 0, 100, 100);
			
			// SetViewportOrgEx(hdc, 200, 15, NULL);
			// SetViewportExtEx(hdc, 2, 2, NULL);
			// Rectangle(hdc, 0, 0, 100, 100);
			
			// SetViewportOrgEx(hdc, 60, 60, NULL);
			// Rectangle(hdc, 0, 0, 100, 100);
			
			// SetViewportOrgEx(hdc, 80, 80, NULL);
			// Rectangle(hdc, 0, 0, 100, 100);
			
			// SetViewportOrgEx(hdc, 100, 100, NULL);
			// Rectangle(hdc, 0, 0, 100, 100);
			
			SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
			SetWindowOrgEx(hdc, -cxClient / 2, -cyClient / 2, NULL);
			TextOut(hdc,  -cxClient / 2, -cyClient / 2, "HELLO", 5);
			
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
