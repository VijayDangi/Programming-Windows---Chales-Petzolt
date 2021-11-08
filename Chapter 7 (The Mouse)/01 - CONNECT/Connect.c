#include <windows.h>

#define MAXPOINTS 1000

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR       szAppName[] = TEXT("Connect");
	HWND        hWnd;
	MSG         msg;
	WNDCLASSEX  wndclass;
	
	wndclass.cbSize        = sizeof(WNDCLASSEX);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName  = NULL;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance     = hInstance;
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK);
		return(0);
	}
	
	hWnd = CreateWindow(
				szAppName,
				TEXT("Connect the points mouse demo"),
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
			
	if(!hWnd)
	{
		MessageBox(NULL, TEXT("Error while creating window."), szAppName, MB_OK);
		return(0);
	}
	
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
	static POINT  pt[MAXPOINTS];
	static int    iCount;
	HDC           hDC;
	int           i, j;
	PAINTSTRUCT   ps;
	
	switch(message)
	{
		case WM_LBUTTONDOWN:
			iCount = 0;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		
		case WM_MOUSEMOVE:
			if( (wParam & MK_LBUTTON) && (iCount < 1000) )
			{
				pt[iCount].x = LOWORD(lParam);
				pt[iCount].y = HIWORD(lParam);
				iCount++;
				
				hDC = GetDC(hWnd);
					SetPixel(hDC, LOWORD(lParam), HIWORD(lParam), 0);
				ReleaseDC(hWnd, hDC);
			}
			break;
		
		case WM_LBUTTONUP:
			InvalidateRect(hWnd, NULL, FALSE);
			return(0);
		
		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			
			SetCursor(LoadCursor(NULL, IDC_WAIT));
			ShowCursor(TRUE);
			
			for(i= 0; i < iCount - 1; i++)
				for(j = i + 1; j < iCount; j++)
				{
					MoveToEx(hDC, pt[i].x, pt[i].y, NULL);
					LineTo  (hDC, pt[j].x, pt[j].y);
				}
				
			ShowCursor(FALSE);
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			
			EndPaint(hWnd, &ps);
		
			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
