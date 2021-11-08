/*-----------------------------------------
    PatBlt.c :- PatBlt Demonstrations
-------------------------------------------*/

/*
 *  MSDN : Raster Operations
 */

//Headers
#include <windows.h>

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR       szAppName[] = TEXT("Stretch");
    HWND        hwnd;
    MSG         msg;
    WNDCLASSEX  wndclass;
    
    //code
	wndclass.cbSize 		= sizeof(WNDCLASSEX);
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc    = WndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.lpszClassName  = szAppName;
    wndclass.lpszMenuName   = NULL;
    wndclass.hInstance      = hInstance;
    wndclass.hIcon          = LoadIcon(NULL, IDI_INFORMATION);
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.hIconSm 		= LoadIcon(NULL, IDI_INFORMATION);
    
    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("StretchBlt Demo"),
                WS_OVERLAPPEDWINDOW,
                300,
                300,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                NULL,
                NULL,
                hInstance,
                NULL
            );
            
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return((int)msg.wParam);
}

//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //variable declarations
    static int  cxClient, cyClient;
    HDC         hdcClient, hdcWindow;
    PAINTSTRUCT ps;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            return(0);
            
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return(0);
            
        case WM_PAINT:					   
            hdcClient = BeginPaint(hwnd, &ps);
            
                SelectObject(hdcClient, CreateHatchBrush(HS_DIAGCROSS, RGB(255, 0, 0)));
                
                // PatBlt(hdcClient, 0, 0, cxClient, cyClient, BLACKNESS);
                // PatBlt(hdcClient, 0, 0, cxClient, cyClient, WHITENESS);
                // PatBlt(hdcClient, 0, 0, cxClient, cyClient, DSTINVERT);
                PatBlt(hdcClient, 0, 0, cxClient, cyClient, PATINVERT);
                // PatBlt(hdcClient, 0, 0, cxClient, cyClient, PATCOPY);
                // PatBlt(hdcClient, 0, 0, cxClient, cyClient, 0x0500A9); /* ~(P | D) */
                // PatBlt(hdcClient, 0, 0, cxClient, cyClient, 0x0A0329); /* ~P & D */
                // PatBlt(hdcClient, 0, 0, cxClient, cyClient, 0x0F0001); /* ~P */
                // PatBlt(hdcClient, 0, 0, cxClient, cyClient, 0xFA0089); /* P | D */
                
                DeleteObject(SelectObject(hdcClient, GetStockObject(WHITE_BRUSH)));
            
            EndPaint(hwnd, &ps);
            return(0);
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}

