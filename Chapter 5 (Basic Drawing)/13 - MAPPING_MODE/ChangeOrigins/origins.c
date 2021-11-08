#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR iCmdLine, int iCmdShow)
{
	TCHAR      szAppName[] = TEXT("MAPMODE");
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
						TEXT("MAPPING MODE"),
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
	POINT       pt;
    HDC         hDC;
    PAINTSTRUCT ps;
    static int  cxClient, cyClient;
    TCHAR       szBuffer[128];
	
	switch(message)
	{
        case WM_CREATE:
            
            return(0);
            
		case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			return(0);
		}
		
		case WM_PAINT:
		{
			hDC = BeginPaint(hWnd, &ps);
			
                SetViewportOrgEx(hDC, cxClient/2, cyClient/2, NULL);
                SetWindowOrgEx(hDC, -cxClient/2, -cyClient/2, NULL);
                
                                
                //
                MoveToEx(hDC, -100, -100, NULL);
                LineTo(hDC, cxClient, -100);
                
                MoveToEx(hDC, -100, -100, NULL);
                LineTo(hDC, -cxClient,-100);
                
                MoveToEx(hDC, -100, -100, NULL);
                LineTo(hDC, -100, -cyClient);
                
                MoveToEx(hDC, -100, -100, NULL);
                LineTo(hDC, -100, cyClient);
                
                GetViewportOrgEx(hDC, &pt);
                TextOut(hDC, -cxClient, -cyClient, szBuffer, wsprintf(szBuffer, TEXT("xViewOrg = %d, yViewOrg = %d"), pt.x, pt.y));
                
                GetWindowOrgEx(hDC, &pt);
                TextOut(hDC, -cxClient, -cyClient + 20, szBuffer, wsprintf(szBuffer, TEXT("xWinOrg = %d, yWinOrg = %d"), pt.x, pt.y));
                            
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

