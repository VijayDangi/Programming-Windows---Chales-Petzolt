/*********************************************************
    POPPAD1.c : Popup Editor using child window edit box
*********************************************************/

//Header
#include <windows.h>

//macro
#define ID_EDIT 1

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
TCHAR szAppName[] = TEXT("PopPad1");

//WinMain()
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
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
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hInstance     = hInstance;
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    
    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                szAppName,
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //variable declarations
    static HWND hwndEdit;
    
    switch(message)
    {
        case WM_CREATE:
            hwndEdit = CreateWindow(
                            TEXT("EDIT"),
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
            
        case WM_COMMAND:
            if(LOWORD(wParam) == ID_EDIT)
                if(HIWORD(wParam) == EN_ERRSPACE || HIWORD(wParam) == EN_MAXTEXT)
                    MessageBox(hwnd, TEXT("Edit control out of space."), szAppName, MB_OK | MB_ICONSTOP);
             
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}
