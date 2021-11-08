/*---------------------------------------------
    About3.c :- About Box Demo Program No. 3 
---------------------------------------------*/

//Headers
#include <windows.h>
#include "Resource.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK EllipPushWndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR    szAppName[] = TEXT("About3");
    MSG      msg;
    HWND     hwnd;
    WNDCLASS wndclass;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = szAppName;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(hInstance, szAppName);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = EllipPushWndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.lpszClassName = TEXT("EllipPush");;
    wndclass.lpszMenuName  = NULL;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = NULL;
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
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
                    
                default:
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
    // TCHAR szClassName[128];
    
    //code
    switch(message)
    {
        case WM_INITDIALOG:
            // GetClassName(hDlg, szClassName, 128);
            // MessageBox(hDlg, szClassName, szClassName, MB_OK);
            return(TRUE);
            
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                case IDCANCEL:
                    EndDialog(hDlg, 0);
                    return(TRUE);
                    
                default:
                    break;
            }
            break;
    }
    
    return(FALSE);
}


//EllipPushWndProc
LRESULT CALLBACK EllipPushWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //variable declarations
    TCHAR       szText[40];
    HBRUSH      hBrush;
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rect;
    
    //code
    switch(message)
    {
        case WM_PAINT:
            GetClientRect(hwnd, &rect);
            GetWindowText(hwnd, szText, sizeof(szText));
            
            hdc = BeginPaint(hwnd, &ps);
            
                hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
                hBrush = (HBRUSH) SelectObject(hdc, hBrush);
                SetBkColor(hdc, GetSysColor(COLOR_WINDOW));
                SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                
                Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
                DrawText(hdc, szText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
                
                DeleteObject(SelectObject(hdc, hBrush));
                
                if(hwnd == GetFocus())
                {
                    rect.left += rect.right/10;
                    rect.right -=  rect.left;
                    rect.top += rect.bottom / 10;
                    rect.bottom -= rect.top;
                    
                    SelectObject(hdc, GetStockObject(NULL_BRUSH));
                    SelectObject(hdc, CreatePen(PS_DASH, 0, 0));
                    Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
                    DeleteObject(SelectObject(hdc, GetStockObject(BLACK_PEN)));
                }
            
            EndPaint(hwnd, &ps);
            
            break;
        
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
            break;

        case WM_LBUTTONDOWN:
            SetFocus(hwnd);
            // InvalidateRect(hwnd, NULL, TRUE);
            // UpdateWindow(hwnd);
            break;
        
        case WM_KEYUP:
            if(wParam != VK_SPACE)
                break;
            //fall through
        case WM_LBUTTONUP:
            
            if(hwnd == GetFocus())
            {
                SendMessage(
                    GetParent(hwnd), 
                    WM_COMMAND, 
                    (WPARAM)GetWindowLong(hwnd, GWL_ID),
                    (LPARAM)hwnd
                );
            }
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));    
}
