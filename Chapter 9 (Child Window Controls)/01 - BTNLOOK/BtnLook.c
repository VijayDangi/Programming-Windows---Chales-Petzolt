/**********
    BTNLOOK.c - Button Look Program
**********/

//Header
#include <windows.h>

//global variable declarations
struct
{
    int    iStyle;
    TCHAR *szText;
}
button[] = {
        BS_PUSHBUTTON,              TEXT("BS_PUSHBUTTON"),
        BS_DEFPUSHBUTTON,           TEXT("BS_DEFPUSHBUTTON"),
        BS_CHECKBOX,                TEXT("BS_CHECKBOX"),
        BS_AUTOCHECKBOX,            TEXT("BS_AUTOCHECKBOX"),
        BS_RADIOBUTTON,             TEXT("BS_RADIOBUTTON"),
        BS_3STATE,                  TEXT("BS_3STATE"),
        BS_AUTO3STATE,              TEXT("BS_AUTO3STATE"),
        BS_GROUPBOX,                TEXT("BS_GROUPBOX"),
        BS_AUTORADIOBUTTON,         TEXT("BS_AUTORADIOBUTTON"),
        BS_OWNERDRAW,               TEXT("BS_OWNERDRAW")
};

//macro
#define NUM (sizeof (button) / sizeof (button[0]))

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR       szAppName[] = TEXT("BtnLook");
    HWND        hwnd;
    MSG         msg;
    WNDCLASSEX  wndclass;
    
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
    wndclass.hInstance     = hInstance;
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    
    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("Button Look"),
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                HWND_DESKTOP,
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
    static HWND  hwndButton[NUM];
    static RECT  rect;
    
    static TCHAR szTop[]    = TEXT("message            wParam       lParam"),
                 szUnd[]    = TEXT("_______            ______       _______"),
                 szFormat[] = TEXT("%-16s%04X-%04X    %04X-%04X"),
                 szBuffer[50];
                 
    static int   cxChar, cyChar;
    HDC          hdc;
    PAINTSTRUCT  ps;
    int          i;
    
    
    //code
    switch(message)
    {
        case WM_CREATE:
            cxChar = LOWORD(GetDialogBaseUnits());
            cyChar = HIWORD(GetDialogBaseUnits());
            
            for(i = 0; i < NUM; i++)
            {
                hwndButton[i] = CreateWindow(
                                    TEXT("BUTTON"),
                                    button[i].szText,
                                    WS_CHILD | WS_VISIBLE | button[i].iStyle,
                                    cxChar, cyChar * (1 + 2 * i),
                                    20 * cxChar, 7 * cyChar / 4,
                                    hwnd,
                                    (HMENU) i,
                                    ((LPCREATESTRUCT)lParam)->hInstance,
                                    NULL
                                );
            }
            break;
            
            
        case WM_SIZE:
            rect.left   = 24 * cxChar;
            rect.top    = 2 * cyChar;
            rect.right  = LOWORD(lParam);
            rect.bottom = HIWORD(lParam);
            break;
    
        case WM_PAINT:
            InvalidateRect(hwnd, &rect, TRUE);
            
            hdc = BeginPaint(hwnd, &ps);
                
                SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
                SetBkMode(hdc, TRANSPARENT);
                
                TextOut(hdc, 24 * cxChar, cyChar, szTop, lstrlen(szTop));
                TextOut(hdc, 24 * cxChar, cyChar, szUnd, lstrlen(szUnd));
                                
            EndPaint(hwnd, &ps);
            break;
    
    
        case WM_DRAWITEM:
        case WM_COMMAND:
            ScrollWindow(hwnd, 0, -cyChar, &rect, &rect);
            
            hdc = GetDC(hwnd);
            
                SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
                
                TextOut(hdc, 24 * cxChar, cyChar * (rect.bottom / cyChar - 1),
                        szBuffer,
                        wsprintf(szBuffer, szFormat,
                                 message == WM_DRAWITEM ? TEXT("WM_DRAWITEM") : TEXT("WM_COMMAND"),
                                 HIWORD(wParam), LOWORD(wParam),
                                 HIWORD(lParam), LOWORD(lParam)
                        )
                );
            
            ReleaseDC(hwnd, hdc);
            
            ValidateRect(hwnd, &rect);
            break;
    
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}
