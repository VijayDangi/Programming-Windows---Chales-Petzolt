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
						TEXT("MM_ISOTROPIC example"), //Caption title
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
			
                SetTextAlign(hDC, TA_RIGHT | TA_TOP);
                SetTextColor(hDC, RGB(255, 0, 0));
                TextOut(hDC, cxClient, 10, TEXT("SetWindowOrgEx(hDC, 0, 32767, NULL)"), lstrlen(TEXT("SetWindowOrgEx(hDC, 0, 32767, NULL)")));
                
                SetTextColor(hDC, RGB(0, 255, 0));
                TextOut(hDC, cxClient, 30, TEXT("SetViewportOrgEx(hDC, 0, cyClient, NULL)"), lstrlen(TEXT("SetViewportOrgEx(hDC, 0, cyClient, NULL)")));
                
                SetTextColor(hDC, RGB(0, 0, 255));
                TextOut(hDC, cxClient, 50, TEXT("SetViewportExtEx(hDC, cxClient/2, -cyClient/2, NULL)"), lstrlen(TEXT("SetViewportExtEx(hDC, cxClient/2, -cyClient/2, NULL)")));
                TextOut(hDC, cxClient, 70, TEXT("SetViewportOrgEx(hDC, cxClient/2, cyClient/2, NULL)"), lstrlen(TEXT("SetViewportOrgEx(hDC, cxClient/2, cyClient/2, NULL)")));
            
                SelectObject(hDC, GetStockObject(DC_PEN));
            
                //ISOTROPIC
                SetMapMode(hDC, MM_ISOTROPIC); //set mapping mode
                SetWindowExtEx(hDC, 32767, 32767, NULL); //set window extent
                SetViewportExtEx(hDC, cxClient, -cyClient, NULL); //set window viewport
                                
                SetDCPenColor(hDC, RGB(255, 0, 0));
                SetWindowOrgEx(hDC, 0, 32767, NULL); //set window origin
                MoveToEx(hDC,  1000, 32767-1000, NULL);
                LineTo  (hDC, 32767, 32767-1000);
                MoveToEx(hDC,  1000, 32767-1000, NULL);
                LineTo  (hDC,  1000,  1000);
                SetWindowOrgEx(hDC, 0, 0, NULL); //reset window origin
                
                SetDCPenColor(hDC, RGB(0, 255, 0));
                SetViewportOrgEx(hDC, 0, cyClient, NULL); //set viewport origin
                MoveToEx(hDC,   500,   500, NULL);
                LineTo  (hDC, 32767,   500);
                MoveToEx(hDC,   500,   500, NULL);
                LineTo  (hDC,   500, 32767);
                
                SetDCPenColor(hDC, RGB(0, 0, 255));
                SetViewportExtEx(hDC, cxClient/2, -cyClient/2, NULL); //set viewport extent
                SetViewportOrgEx(hDC, cxClient/2, cyClient/2, NULL); //set viewport origin
                MoveToEx(hDC,   500,   500, NULL);
                LineTo  (hDC, 32767,   500);
                MoveToEx(hDC,   500,   500, NULL);
                LineTo  (hDC,   500, 32767);
            
                //ANISOTROPIC
                SetMapMode(hDC, MM_ANISOTROPIC); //set mapping mode
                SetWindowExtEx(hDC, 32767, 32767, NULL); //set window extent
                SetViewportExtEx(hDC, cxClient, -cyClient, NULL); //set window viewport
                                
                SetDCPenColor(hDC, RGB(255, 0, 0));
                SetWindowOrgEx(hDC, 0, 32767, NULL); //set window origin
                MoveToEx(hDC,  4000, 32767-4000, NULL);
                LineTo  (hDC, 32767, 32767-4000);
                MoveToEx(hDC,  4000, 32767-4000, NULL);
                LineTo  (hDC,  4000,  4000);
                SetWindowOrgEx(hDC, 0, 0, NULL); //reset window origin
                
                SetDCPenColor(hDC, RGB(0, 255, 0));
                SetViewportOrgEx(hDC, 0, cyClient, NULL); //set viewport origin
                MoveToEx(hDC,  3500,  3500, NULL);
                LineTo  (hDC, 32767,  3500);
                MoveToEx(hDC,  3500,  3500, NULL);
                LineTo  (hDC,  3500, 32767);
                
                SetDCPenColor(hDC, RGB(0, 0, 255));
                SetViewportExtEx(hDC, cxClient/2, -cyClient/2, NULL); //set viewport extent
                SetViewportOrgEx(hDC, cxClient/2, cyClient/2, NULL); //set viewport origin
                MoveToEx(hDC,  3500,  3500, NULL);
                LineTo  (hDC, 32767,  3500);
                MoveToEx(hDC,  3500,  3500, NULL);
                LineTo  (hDC,  3500, 32767);
                
                
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
