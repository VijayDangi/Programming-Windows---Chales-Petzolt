/*
 * Displays Color Under Cursor
 */

#include <windows.h>

#define ID_TIMER 1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //function declaration
    void FindWindowSize(int *, int *);
    
    //variable declaration
    static TCHAR szAppName[] = TEXT("WhatColor");
    HWND         hWnd;
    MSG          msg;
    WNDCLASSEX   wndclass;
    int          cxWindow, cyWindow;
    
    //code
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
        MessageBox(NULL, TEXT("Error while registering class"), szAppName, MB_OK);
        return(0);
    }
    
    FindWindowSize(&cxWindow, &cyWindow);
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("What Color"),
                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER,
                // WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                cxWindow,
                cyWindow,
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
    //variable declaration
    static COLORREF cr, crLast;
    static HDC      hDCScreen;
    HDC             hDC;
    PAINTSTRUCT     ps;
    POINT           pt;
    RECT            rc;
    TCHAR           szBuffer[16];
    
    //code
    switch(message)
    {
        case WM_CREATE:
            hDCScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
            SetTimer(hWnd, ID_TIMER, 100, NULL);
            return(0);
            
        case WM_TIMER:
            GetCursorPos(&pt);
            cr = GetPixel(hDCScreen, pt.x, pt.y);
            
            //SetPixel(hDCScreen, pt.x, pt.y, 0);
            
            if(cr != crLast)
            {
                crLast = cr;
                InvalidateRect(hWnd, NULL, FALSE);
            }
            return(0);
            
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
            
                GetClientRect(hWnd, &rc);
                
                wsprintf(szBuffer, TEXT("  %02X %02X %02X  "),GetRValue(cr), GetGValue(cr), GetBValue(cr));
                
                DrawText(hDC, szBuffer, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            
                // Rectangle(hDCScreen, rc.left, rc.top, rc.right, rc.bottom);
            
            EndPaint(hWnd, &ps);
            return(0);
            
        case WM_DESTROY:
            DeleteDC(hDCScreen);
            KillTimer(hWnd, ID_TIMER);
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}

void FindWindowSize(int *pCxWindow, int *pCyWindow)
{
    //variable declaration
    HDC        hDCScreen;
    TEXTMETRIC tm;
    
    //code
    hDCScreen = CreateIC(TEXT("DISPLAY"), NULL, NULL, NULL);
        GetTextMetrics(hDCScreen, &tm);
    DeleteDC(hDCScreen);
    
    *pCxWindow =  2 * GetSystemMetrics(SM_CXBORDER) +
                 12 * tm.tmAveCharWidth;
                 
    *pCyWindow =  2 * GetSystemMetrics(SM_CYBORDER) +
                      GetSystemMetrics(SM_CYCAPTION) +
                      2 * tm.tmHeight;
}
