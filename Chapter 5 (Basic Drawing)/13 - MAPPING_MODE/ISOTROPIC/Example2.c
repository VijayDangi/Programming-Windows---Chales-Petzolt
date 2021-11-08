/*
 * System Metrics Display
 */
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR iCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("MapMode");
	HWND         hWnd;
	MSG          msg;
	WNDCLASSEX   wndclass;
	
	wndclass.cbSize        = sizeof(wndclass);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName  = NULL;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registering class"), szAppName, MB_ICONERROR);
		return (0);
	}
	
	hWnd = CreateWindow(szAppName, //class name
						TEXT("Change Mapping Mode and Text Display"), //Caption title
						WS_OVERLAPPEDWINDOW, // style
						CW_USEDEFAULT, //x - position
						CW_USEDEFAULT, //y - position
						CW_USEDEFAULT, // width
						CW_USEDEFAULT, // height
						NULL, //parent window handle
						NULL, //menu
						hInstance, //handle of instance of program
						NULL //lParam for creation
					);
	
	ShowWindow(hWnd, iCmdShow); //display window
	UpdateWindow(hWnd); //WM_PAINT -> paint window
	
	//Message Loop
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC          hDC;
    PAINTSTRUCT  ps;
    POINT        pt;
    static int   cxClient, cyClient;
    static TCHAR str[] = TEXT("Text is display is 1 inch from left edge and 1 inch from top edge");
    static TCHAR str1[] = TEXT("SetWindowOrgEx() : Text is display is 1 inch from left edge and 1 inch from top edge");
    TCHAR szBuffer[128];
    SIZE size;
    RECT rc;
	switch(message)
	{
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return(0);
            
		
		case WM_PAINT:
		{   
            hDC = BeginPaint(hWnd, &ps);
			
                SetMapMode(hDC, MM_ISOTROPIC);
                SetWindowExtEx(hDC, 32767, 32767, NULL);
                SetViewportExtEx(hDC, cxClient, -cyClient, NULL);
                SetViewportOrgEx(hDC, 0, cyClient, NULL);
                
                GetClientRect(hWnd, &rc);
                // SetMapMode(hDC, MM_ISOTROPIC);
                // SetWindowExtEx(hDC, 32767, 32767, NULL);
                // SetViewportExtEx(hDC, rc.right, -rc.bottom, NULL);
                // SetViewportOrgEx(hDC, 0, cyClient, NULL);
               
                
                GetWindowExtEx(hDC, &size);
                TextOut(hDC, 0, 2000, szBuffer, wsprintf(szBuffer, TEXT("Window x = %d, y = %d"), size.cx, size.cy));
                
                GetViewportExtEx(hDC, &size);
                TextOut(hDC, 0, 4000, szBuffer, wsprintf(szBuffer, TEXT("Viewport x = %d, y = %d"), size.cx, size.cy));
                
                TextOut(hDC, 0, 32767, szBuffer, wsprintf(szBuffer, TEXT("x = 0, y = 32767")));
                TextOut(hDC, 0, 32767/2, szBuffer, wsprintf(szBuffer, TEXT("RECT right = %d, height = %d"), rc.right, rc.bottom));
                
            EndPaint(hWnd, &ps);
            
			return(0);
		}
		
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return(0);
		}
	}
	
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
