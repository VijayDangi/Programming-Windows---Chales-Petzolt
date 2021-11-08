/******************
    OWNDRAW.c
******************/

//Headers
#include <windows.h>

//macro
#define ID_SMALLER   1
#define BTN_WIDTH    (8 * cxChar)
#define BTN_HEIGHT   (4 * cyChar)
#define IDI_ICON     100
#define ID_TIMER     1000
//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
HINSTANCE ghInst;

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR      szAppName[] = TEXT("OwnDraw");
    HWND       hwnd;
    MSG        msg;
    WNDCLASSEX wndclass;
    
    //code
    ghInst = hInstance;
    
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
                TEXT("Owner-Draw Button Demo"),
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
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //variable declarations
    static HWND      hwndButton;
    static int       cxChar, cyChar;
    static int       i = 0;
    
    switch(message)
    {
        case WM_CREATE:
            cxChar = LOWORD(GetDialogBaseUnits());
            cyChar = HIWORD(GetDialogBaseUnits());
            
                //create the owner-draw pushbuttons
            hwndButton = CreateWindow(
                                TEXT("BUTTON"), TEXT(""),
                                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_ICON,
                                0, 0, BTN_WIDTH, BTN_HEIGHT,
                                hwnd, (HMENU) ID_SMALLER, ghInst, NULL
                            );
            

            SetTimer(hwnd, ID_TIMER, 500, NULL);
            break;
            
        case WM_TIMER:
            if(i%10 == 0)
            {   
                SendMessage(hwndButton, 
                        BM_SETIMAGE,
                        (WPARAM) IMAGE_ICON,
                        (LPARAM)(HANDLE) LoadIcon(ghInst, MAKEINTRESOURCE(IDI_ICON)));;
            }
            else            
            {
                SendMessage(hwndButton, 
                        BM_SETIMAGE,
                        (WPARAM) IMAGE_ICON,
                        (LPARAM)(HANDLE) i);
            }
            i++;
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}
