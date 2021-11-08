/*----------------------------------------------
    ABOUT1.c :- About Box Demo Program No. 1
----------------------------------------------*/

//Headers
#include <windows.h>
#include "Resource.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR    szAppName[] = TEXT("About1");
    MSG      msg;
    HWND     hwnd;
    WNDCLASS wndclass;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(hInstance, szAppName);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName  = szAppName;
    wndclass.lpszClassName = szAppName;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("About Box Demo Program"),
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
    static HINSTANCE hInstance;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
            break;
            
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_APP_ABOUT:
                    DialogBox(hInstance, TEXT("AboutBox"), hwnd, AboutDlgProc);
                    break;
            }
            break;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return(DefWindowProc(hwnd, message, wParam, lParam));
}

//AboutDlgProc()
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    //code
    switch(message)
    {
        case WM_INITDIALOG:
                //If the dialog box procedure returns TRUE, (in WM_INITDIALOG) ,
                //Windows sets the input focus to the first child window control
                //in the dialog box that has a WS_TABSTOP style.
            return(TRUE); 
            
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:  //for Escape key Windows  sends IDCANCEL
                    EndDialog(hDlg, 0);
                    return(TRUE);
            }
            break;
    }
    
    return(FALSE);
}
