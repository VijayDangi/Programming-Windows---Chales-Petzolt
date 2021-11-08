/*****************************************
    POPMENU.c - Popup Menu Demonstration
*****************************************/

//Header
#include <windows.h>
#include "resource.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
HINSTANCE ghInst;
TCHAR     szAppName[] = TEXT("PopMenu");

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = 0;
    wndclass.hIcon         = LoadIcon(NULL, szAppName);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.hInstance     = hInstance;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR | MB_OK);
        exit(0);
    }
    
    ghInst = hInstance;
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("Popup Menu Demonstration"),
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
    static HMENU hMenu;
    static int   idColor[] = {
            WHITE_BRUSH, LTGRAY_BRUSH, GRAY_BRUSH, DKGRAY_BRUSH, BLACK_BRUSH
    };
    static int   iSelection = IDM_BKGND_WHITE;
    POINT        point;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            hMenu = LoadMenu(ghInst, szAppName);
            hMenu = GetSubMenu(hMenu, 0);
            break;
            
        case WM_RBUTTONUP:
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);
            ClientToScreen(hwnd, &point);
            
            TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hwnd, NULL);  //for floating menu / context menu
            break;
            
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
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
                    
                    /**
                        Note: Login below assumes that IDM_BKGND_WHITE through IDM_BKGND_BLACK are consecutive numbers
                        int order show here
                     **/
                case IDM_BKGND_WHITE:
                case IDM_BKGND_LTGRAY:
                case IDM_BKGND_GRAY:
                case IDM_BKGND_DKGRAY:
                case IDM_BKGND_BLACK:
                    CheckMenuItem (hMenu, iSelection, MF_UNCHECKED);
                    iSelection = LOWORD(wParam);
                    CheckMenuItem (hMenu, iSelection, MF_CHECKED);
                    
                    SetClassLong (
                        hwnd,
                        GCL_HBRBACKGROUND,
                        (LONG)GetStockObject(idColor[LOWORD(wParam) - IDM_BKGND_WHITE])
                    );
                    
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                    
                case IDM_APP_ABOUT:
                    MessageBox(hwnd, TEXT("Popup Menu Demonstration Program\n") 
                                     TEXT("(c) Charles Petzold, 1998"), szAppName, MB_ICONEXCLAMATION | MB_OK);
                                     
                    break;
                    
                case IDM_APP_EXIT:
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    break;
                    
                case IDM_APP_HELP:
                    MessageBox(hwnd, TEXT("Help not yet implemented!!"), szAppName, MB_ICONEXCLAMATION | MB_OK);
                    break;
            }
            break;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}
