/*----------------------------------------------
  PoePoem.c -- Demonstrates Custom Resource
----------------------------------------------*/

//Header
#include <windows.h>
#include "resource.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
HINSTANCE ghInst;

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR    szAppName[16], szCaption[64], szErrMsg[64];
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    
    //code
    LoadString (hInstance, IDS_APPNAME, szAppName, sizeof(szAppName)/sizeof(TCHAR));
    LoadString (hInstance, IDS_CAPTION, szCaption, sizeof(szCaption)/sizeof(TCHAR));

    ghInst = hInstance;
    
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = NULL;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(hInstance, szAppName);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    
    if(!RegisterClass(&wndclass))
    {    
        LoadStringA(hInstance, IDS_APPNAME, (char*) szAppName, sizeof(szAppName));
        LoadStringA(hInstance,  IDS_ERRMSG, (char*)  szErrMsg, sizeof(szErrMsg));
    
        MessageBoxA(NULL, (char*) szErrMsg, (char*) szAppName, MB_ICONERROR);
        
        exit(0);
    }
    hwnd = CreateWindow(
                szAppName,
                szCaption,
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
    static char   *pText;
    static HGLOBAL hResource;
    static HWND    hScroll;
    static int     iPosition, cxChar, cyChar, cyClient, iNumLines, xScroll;
    HDC            hdc;
    PAINTSTRUCT    ps;
    RECT           rect;
    TEXTMETRIC     tm;
    TCHAR szErrMsg[64];
    
    //code
    switch(message)
    {
        case WM_CREATE:
            hdc = GetDC(hwnd);
                GetTextMetrics(hdc, &tm);
                cxChar = tm.tmAveCharWidth;
                cyChar = tm.tmHeight + tm.tmExternalLeading;
            ReleaseDC(hwnd, hdc);
            
            xScroll = GetSystemMetrics(SM_CXVSCROLL);
            
            hScroll = CreateWindow(
                        TEXT("scrollbar"),
                        NULL,
                        WS_CHILD | WS_VISIBLE | SBS_VERT,
                        0, 0, 0, 0,
                        hwnd,
                        (HMENU) 1,
                        ghInst,
                        NULL
                    );
                    
            hResource = LoadResource(ghInst,
                                     FindResource(ghInst, TEXT("AnnabelLee"), TEXT("TEXT"))
                              );
             
            pText = (char*) LockResource(hResource);
            iNumLines = 0;
            

            // while( (*pText) != TEXT('\\') && (*pText) != TEXT('\0'))
            while((*pText) != TEXT('\0'))
            {
                if((*pText) == '\n')
                    iNumLines++;
                pText = AnsiNext(pText);
            }
            // *pText = '\0';
            
            SetScrollRange(hScroll, SB_CTL, 0, iNumLines, FALSE);
            SetScrollPos  (hScroll, SB_CTL, 0, FALSE);
            break;
            
            
        case WM_SIZE:
            MoveWindow(
                    hScroll,
                    LOWORD(lParam) - xScroll,
                    0,
                    xScroll,
                    cyClient = HIWORD(lParam),
                    TRUE
                );
                
            SetFocus(hwnd);
            break;
            
        case WM_SETFOCUS:
            SetFocus(hScroll);
            break;
            
        case WM_VSCROLL:
            switch(wParam)
            {
                case SB_TOP:
                    iPosition = 0;
                    break;
                    
                case SB_BOTTOM:
                    iPosition = iNumLines;
                    break;
                    
                case SB_LINEUP:
                    iPosition = iPosition - 1;
                    break;
                    
                case SB_LINEDOWN:
                    iPosition = iPosition + 1;
                    break;
                    
                case SB_PAGEUP:
                    iPosition = iPosition - (cyClient / cyChar);
                    break;
                    
                case SB_PAGEDOWN:
                    iPosition = iPosition + (cyClient / cyChar);
                    break;
                    
                case SB_THUMBPOSITION:
                    iPosition = LOWORD(lParam);
                    break;
            }
            
            iPosition = max(0, min(iPosition, iNumLines));
            
            if(iPosition != GetScrollPos(hScroll, SB_CTL))
            {
                SetScrollPos(hScroll, SB_CTL, iPosition, TRUE);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
            
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            
                pText = (char*) LockResource(hResource);
                
                GetClientRect(hwnd, &rect);
                rect.left = rect.left + cxChar;
                rect.top  = rect.top  + cyChar * (1 - iPosition);
                
                DrawTextA(hdc, pText, -1, &rect, DT_EXTERNALLEADING);
            
            EndPaint(hwnd, &ps);
            break;
            
        case WM_DESTROY:
            FreeResource(hResource);
            PostQuitMessage(0);
            break;
    }
    return(DefWindowProc(hwnd, message, wParam, lParam));
}
