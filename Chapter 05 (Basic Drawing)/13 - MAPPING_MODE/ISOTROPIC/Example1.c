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
                LineTo(hDC, 100, -100);
                Ellipse(hDC, 0, 0, 900, -500);
                pt.x = 10;
                pt.y = 10;
                LPtoDP(hDC, &pt, 1);
                TextOut(hDC, 100, -900, szBuffer, wsprintf(szBuffer, TEXT("x = %d, y = %d"), pt.x, pt.y));
            EndPaint(hWnd, &ps);
            
                
                // {
                    // PAINTSTRUCT ps;
                    // HDC hDC = BeginPaint(hWnd, &ps);
                    // RECT rect;
                    // HFONT hf, hfOld;

                    // GetClientRect(hWnd, &rect);

                    // SetMapMode(hDC, MM_ISOTROPIC);
                    // SetWindowExtEx(hDC, 1250, 1250, NULL);
                    // SetViewportExtEx(hDC, rect.right, rect.bottom, NULL);
                    // SetViewportOrgEx(hDC, 0,0,NULL);

                    // hf = CreateFont(3000/7,
                                  // 3000/30,
                                  // 0,
                                  // 0,
                                  // FW_DEMIBOLD,
                                  // 0,
                                  // 0,
                                  // 0,
                                  // ANSI_CHARSET,
                                  // OUT_TT_PRECIS,
                                  // CLIP_DEFAULT_PRECIS,
                                  // DEFAULT_QUALITY,
                                  // DEFAULT_PITCH,
                                  // "Courier New");

                    // hfOld = SelectObject(hDC, hf);

                    // TextOut(hDC,500,500,"This is a test of font scaling",
                       // strlen("This is a test of font scaling"));
                       
                    // pt.x = 10;
                // // pt.y = 10;
                // // LPtoDP(hDC, &pt, 1);
                // // TextOut(hDC, 100, -900, szBuffer, wsprintf(szBuffer, TEXT("x = %d, y = %d"), pt.x, pt.y));


                    // SelectObject(hDC, hfOld);
                    // DeleteObject(hf);
                    // EndPaint(hWnd, &ps);
                // }

			
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
