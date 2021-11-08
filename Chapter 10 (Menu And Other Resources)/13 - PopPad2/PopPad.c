/*------------------------------------------------------
    POPPAD2 - Popup Editor Version 2 (includes menu)
------------------------------------------------------*/

//Header
#include <windows.h>
#include "resource.h"

//macro
#define ID_EDIT 1

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
TCHAR szAppName[] = TEXT("PopPad2");

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    HACCEL   hAccel;
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = szAppName;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hIcon         = LoadIcon(hInstance, szAppName);
    wndclass.hCursor       = LoadCursor(hInstance, szAppName);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.hInstance     = hInstance;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                szAppName,
                WS_OVERLAPPEDWINDOW,
                GetSystemMetrics(SM_CXSCREEN) / 4,
                GetSystemMetrics(SM_CYSCREEN) / 4,
                GetSystemMetrics(SM_CXSCREEN) / 2,
                GetSystemMetrics(SM_CYSCREEN) / 2,
                NULL,
                NULL,
                hInstance,
                NULL
            );
            
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    
    hAccel = LoadAccelerators(hInstance, szAppName);
    
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        if(!TranslateAccelerator(hwnd, hAccel, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    return((int)msg.wParam);
}

//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //function declarations
    int AskConfirmation(HWND);

    //variable declarations
    static HWND hwndEdit;
    int         iSelect, iEnable;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            hwndEdit = CreateWindow(
                            TEXT("edit"),
                            NULL,
                            WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
                            0, 0, 0, 0,
                            hwnd,
                            (HMENU) ID_EDIT,
                            ((LPCREATESTRUCT)lParam)->hInstance,
                            NULL
                        );
            break;
            
        case WM_SETFOCUS:
            SetFocus(hwndEdit);
            break;
            
        case WM_SIZE:
            MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
            break;
            
        case WM_INITMENUPOPUP:
            if(lParam == 1)
            {
                EnableMenuItem(
                        (HMENU) wParam,
                        IDM_EDIT_UNDO,
                        SendMessage(hwndEdit, EM_CANUNDO, 0, 0) ?
                            MF_ENABLED : MF_GRAYED
                     );
                            
                EnableMenuItem(
                        (HMENU) wParam,
                        IDM_EDIT_PASTE,
                        IsClipboardFormatAvailable(CF_TEXT) ?
                            MF_ENABLED : MF_GRAYED
                    );
                    
                iSelect = SendMessage(hwndEdit, EM_GETSEL, 0, 0);
                
                if(HIWORD(iSelect) == LOWORD(iSelect))
                    iEnable = MF_GRAYED;
                else
                    iEnable = MF_ENABLED;
                    
                EnableMenuItem((HMENU) wParam, IDM_EDIT_CUT,  iEnable);
                EnableMenuItem((HMENU) wParam, IDM_EDIT_COPY,  iEnable);
                EnableMenuItem((HMENU) wParam, IDM_EDIT_CLEAR, iEnable);
                break;
            }
            break;
            
        case WM_COMMAND:
            if(lParam)
            {
                if( LOWORD(lParam) == ID_EDIT &&
                    (HIWORD(wParam) == EN_ERRSPACE || HIWORD(wParam) == EN_MAXTEXT)
                )
                {
                    MessageBox(hwnd, TEXT("Edit control out of space."), szAppName, MB_OK | MB_ICONSTOP);
                }
            }
            else
            {
                switch(LOWORD(wParam))
                {
                    case IDM_FILE_NEW:
                    case IDM_FILE_OPEN:
                    case IDM_FILE_SAVE:
                    case IDM_FILE_SAVE_AS:
                    case IDM_FILE_PRINT:
                        MessageBeep(0);
                        break;
                        
                    case IDM_APP_EXIT:
                        SendMessage(hwnd, WM_CLOSE, 0, 0);
                        break;
                        
                    case IDM_EDIT_UNDO:
                        SendMessage(hwndEdit, WM_UNDO, 0, 0);
                        break;
                    
                    case IDM_EDIT_CUT:
                        SendMessage(hwndEdit, WM_CUT, 0, 0);
                        break;
                        
                    case IDM_EDIT_COPY:
                        SendMessage(hwndEdit, WM_COPY, 0, 0);
                        break;
                        
                    case IDM_EDIT_PASTE:
                        SendMessage(hwndEdit, WM_PASTE, 0, 0);
                        break;
                        
                    case IDM_EDIT_CLEAR:
                        SendMessage(hwndEdit, WM_CLEAR, 0, 0);
                        break;
                        
                    case IDM_EDIT_SELECT_ALL:
                        SendMessage(hwndEdit, EM_SETSEL, 0, -1);
                        break;
                        
                    case IDM_HELP_HELP:
                        MessageBox(hwnd, TEXT("Help not implemented yet"), szAppName, MB_OK | MB_ICONEXCLAMATION);
                        break;
                        
                    case IDM_APP_ABOUT:
                        MessageBox(hwnd, TEXT("PopPad2 (c) Chales Petzold, 1998"), szAppName, MB_OK | MB_ICONEXCLAMATION);
                        break;
                }
            }
            break;
            
        case WM_CLOSE:
            if(IDYES == AskConfirmation(hwnd))
                DestroyWindow(hwnd);
            return(0);
            
        case WM_QUERYENDSESSION:
            if(IDYES == AskConfirmation(hwnd))
                return(1);
            return(0);
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}

//AskConfirmation()
int AskConfirmation(HWND hwnd)
{
    return(MessageBox(hwnd, TEXT("Really want to close PopPad2 ?"), szAppName, MB_YESNO | MB_ICONQUESTION));
}
