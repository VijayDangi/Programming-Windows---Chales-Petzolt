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
						TEXT("MM_ISOTROPIC: Change Pixels Per inch"), //Caption title
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
			
                TextOut(hDC, 1, 1, TEXT("MM_TEXT: inch from left 1 inch from top"), lstrlen(TEXT("MM_TEXT: inch from left 1 inch from top")));
                pt.x = 1;
                pt.y = 1;
                LPtoDP(hDC, &pt, 1);
                wsprintf(szBuffer, TEXT("MM_TEXT: x = %d, y = %d"), pt.x, pt.y);
                MessageBox(hWnd, szBuffer, TEXT(""), MB_OK);
                
                SetMapMode(hDC, MM_ISOTROPIC); //set mapping mode
                SetWindowExtEx(hDC, 16, 16, NULL); //set window extent                                              // ----|
                                                                                                                    //     | --- x = (LOGPIXELSX) / 16; y = (LOGPIXELSY)/16;
                SetViewportExtEx(hDC, GetDeviceCaps(hDC, LOGPIXELSX), GetDeviceCaps(hDC, LOGPIXELSY), NULL);        // ----|
                
                TextOut(hDC, 1, 1, TEXT("MM_ISOTROPIC: inch from left 1 inch from top"), lstrlen(TEXT("MM_TEXT: inch from left 1 inch from top")));
                pt.x = 1;
                pt.y = 1;
                LPtoDP(hDC, &pt, 1);
                wsprintf(szBuffer, TEXT("MM_TEXT: x = %d, y = %d"), pt.x, pt.y);
                MessageBox(hWnd, szBuffer, TEXT(""), MB_OK);
                
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
