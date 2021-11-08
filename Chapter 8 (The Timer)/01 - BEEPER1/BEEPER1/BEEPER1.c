/*
 * Timer Demo Program #1
 */

#include <windows.h>

#define ID_TIMER 1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("Beeper1");
	HWND         hWnd;
	MSG          msg;
	WNDCLASSEX   wndclass;

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

	hWnd = CreateWindow(
				szAppName,
				TEXT("Beeper #1 Time Demo"),
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
	static BOOL fFlipFlop = FALSE;
	HBRUSH      hBrush;
	HDC         hDC;
	PAINTSTRUCT ps;
	RECT        rc;

	switch(message)
	{
		case WM_CREATE:
				SetTimer(hWnd, ID_TIMER, 1000, NULL);
				return(0);

		case WM_TIMER:
			MessageBeep(MB_ICONSTOP);
			fFlipFlop = !fFlipFlop;
			InvalidateRect(hWnd, NULL, FALSE);
			return(0);

		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);

				GetClientRect(hWnd, &rc);
				hBrush = CreateSolidBrush(fFlipFlop ? RGB(255, 0, 0) : RGB(0, 0, 255));
				FillRect(hDC, &rc, hBrush);
				
			EndPaint(hWnd, &ps);
			DeleteObject(hBrush);
			return(0);

		case WM_DESTROY:
			PostQuitMessage(0);
			return(0);
	}

	return(DefWindowProc(hWnd, message, wParam, lParam));
}
