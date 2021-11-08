/*---------------------------------------------------
    NOPOPUPS.c - Demonstartes No-popup Nested Menu
---------------------------------------------------*/

//Header
#include <windows.h>
#include "resource.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR    szAppName[] = TEXT("NoPopups");
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = NULL;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hInstance     = hInstance;
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registring class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("No-popup Nested Menu Demonstration"),
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
    static HMENU hMenuMain, hMenuEdit, hMenuFile;
    HINSTANCE    hInstance;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            hInstance = (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);
            
            hMenuMain = LoadMenu(hInstance, TEXT("MenuMain"));
            hMenuEdit = LoadMenu(hInstance, TEXT("MenuEdit"));
            hMenuFile = LoadMenu(hInstance, TEXT("MenuFile"));
            
            SetMenu(hwnd, hMenuMain);
            break;
            
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_MAIN:
                    SetMenu(hwnd, hMenuMain);
                    break;
                    
                case IDM_FILE:
                    SetMenu(hwnd, hMenuFile);
                    break;
                    
                case IDM_EDIT:
                    SetMenu(hwnd, hMenuEdit);
                    break;
                    
                case IDM_FILE_NEW:
                case IDM_FILE_OPEN:
                case IDM_FILE_SAVE:
                case IDM_FILE_SAVE_AS:
                case IDM_EDIT_UNDO:
                case IDM_EDIT_CUT:
                case IDM_EDIT_COPY:
                case IDM_EDIT_PASTE:
                case IDM_EDIT_CLEAR:
                    MessageBeep(0);
                    break;
            }
            break;
            
        case WM_DESTROY:
            SetMenu(hwnd, hMenuMain);
            DestroyMenu(hMenuFile);
            DestroyMenu(hMenuEdit);
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}
