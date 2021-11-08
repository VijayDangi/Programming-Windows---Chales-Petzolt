/*----------------------------------------
    POORMENU.c - The Poor Person's Menu
----------------------------------------*/

//Headers
#include <windows.h>

#define IDM_SYS_ABOUT  1
#define IDM_SYS_HELP   2
#define IDM_SYS_REMOVE 3

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
static TCHAR szAppName[] = TEXT("PoorMenu");

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    HMENU    hMenu;
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = szAppName;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("The Poor-Person's Menu (System Menu)"),
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
    
    hMenu = GetSystemMenu(hwnd, FALSE);
    
    AppendMenu (hMenu, MF_SEPARATOR, 0,              NULL);
    AppendMenu (hMenu, MF_STRING,    IDM_SYS_ABOUT,  TEXT("About..."));
    AppendMenu (hMenu, MF_STRING,    IDM_SYS_HELP,   TEXT("Help..."));
    AppendMenu (hMenu, MF_STRING,    IDM_SYS_REMOVE, TEXT("Remove Additions"));
    
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return((int)msg.wParam);
}

//WndProc
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //code
    switch(message)
    {
        case WM_SYSCOMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_SYS_ABOUT:
                    MessageBox(hwnd, TEXT("A Poor-Person's Menu Program\n")
                                     TEXT("(c) Charles Petzold, 1998"), szAppName, MB_OK | MB_ICONEXCLAMATION);
                    break;
                    
                case IDM_SYS_HELP:
                    MessageBox(hwnd, TEXT("Help not yet implemented!"), szAppName, MB_OK | MB_ICONEXCLAMATION);
                    break;
                    
                case IDM_SYS_REMOVE:
                    GetSystemMenu(hwnd, TRUE);
                    break;
            }
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return(DefWindowProc(hwnd, message, wParam, lParam));
}
