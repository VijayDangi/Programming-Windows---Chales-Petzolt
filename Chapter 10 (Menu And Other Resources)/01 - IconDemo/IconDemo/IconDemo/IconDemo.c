/*********************************************
 * ICONDEMO.c - Icon Demonstration Program
 *********************************************/

//Header
#include <windows.h>
#include "resource.h"

///global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR      szAppName[] = TEXT("IconDemo");
    HWND       hwnd;
    MSG        msg;
    WNDCLASSEX wndclass;
    
    //code
    wndclass.cbSize        = sizeof(WNDCLASSEX);
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = NULL;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wndclass.hCursor       = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR));
    wndclass.hInstance     = hInstance;
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.hIconSm       = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    
    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class"), szAppName, MB_OK);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("Icon Demo"),
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
    //variable declarations
    static HICON hIcon;
    static int   cxIcon, cyIcon, cxClient, cyClient;
    HDC          hdc;
    HINSTANCE    hInstance;
    PAINTSTRUCT  ps;
    int          x, y;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
            hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
            cxIcon = GetSystemMetrics(SM_CXICON);
            cyIcon = GetSystemMetrics(SM_CYICON);
            break;
            
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            break;
            
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
                
                for(y = 0; y < cyClient; y += cyIcon)
                    for(x = 0; x < cxClient; x += cxIcon)
                        DrawIcon(hdc, x, y, hIcon);
                
            EndPaint(hwnd, &ps);
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}
