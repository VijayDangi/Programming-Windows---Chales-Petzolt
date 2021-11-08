/*-------------------------------------------------
    HelloBit.c - Bitmap Demonstration
-------------------------------------------------*/

//Header
#include <windows.h>
#include "Resource.h"

//global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declaration
    TCHAR     szAppName[] = TEXT("HelloBit");
    HWND      hwnd;
    MSG       msg;
    WNDCLASS  wndclass;
    
    //code    
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc    = WndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = hInstance;
    wndclass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszClassName  = szAppName;
    wndclass.lpszMenuName   = szAppName;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class"), szAppName, MB_ICONERROR | MB_OK);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("HelloBit"),
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
    //variable declaration
    static HBITMAP  hBitmap;
    static HDC      hdcMem;
    static int      cxBitmap, cyBitmap, cxClient, cyClient, iSize = IDM_BIG;
    static TCHAR   *szText = TEXT(" Hello, World! ");
    HDC             hdc;
    HMENU           hMenu;
    int             x, y;
    PAINTSTRUCT     ps;
    SIZE            size;
 
    //code
    switch(message)
    {
        case WM_CREATE:
            hdc = GetDC(hwnd);
                hdcMem = CreateCompatibleDC(hdc);
                
                GetTextExtentPoint32(hdc, szText, lstrlen(szText), &size);
                cxBitmap = size.cx;
                cyBitmap = size.cy;
                hBitmap = CreateCompatibleBitmap(hdc, cxBitmap, cyBitmap);
                
            ReleaseDC(hwnd, hdc);
            
            SelectObject(hdcMem, hBitmap);
            TextOut(hdcMem, 0, 0, szText, lstrlen(szText));
            
            return(0);
           
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return(0);
            
        case WM_COMMAND:
            hMenu = GetMenu(hwnd);
            
            switch(LOWORD(wParam))
            {
                case IDM_BIG:
                case IDM_SMALL:
                    CheckMenuItem(hMenu, iSize, MF_UNCHECKED);
                    iSize = LOWORD(wParam);
                    CheckMenuItem(hMenu, iSize, MF_CHECKED);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
            }
            return(0);
            
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
                
                switch(iSize)
                {
                    case IDM_BIG:
                        StretchBlt(hdc, 0, 0, cxClient, cyClient,
                                    hdcMem, 0, 0, cxBitmap, cyBitmap, SRCCOPY);
                        
                        break;
                    
                    case IDM_SMALL:
                        for(y = 0; y < cyClient; y = y + cyBitmap)
                            for(x = 0; x < cxClient; x = x + cxBitmap)
                            {
                                BitBlt(hdc, x, y, cxBitmap, cyBitmap,
                                    hdcMem, 0, 0, SRCCOPY);
                            }
                        
                        break;
                }
                
            EndPaint(hwnd, &ps);
            return(0);
            
        case WM_DESTROY:
            DeleteObject(hdcMem);
            DeleteObject(hBitmap);
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}

