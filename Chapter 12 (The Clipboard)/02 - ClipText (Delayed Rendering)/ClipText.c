/*----------------------------------------------
    ClipText.c :- The Clipboard and Text
----------------------------------------------*/

//Headers
#include <windows.h>
#include "Resource.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
#ifdef UNICODE  //Unicode version

#define CF_TCHAR CF_UNICODETEXT
TCHAR szDefaultText[] = TEXT("Default Text - Unicode Version");
TCHAR szCaption[]     = TEXT("Clipboard Text Transfers - Unicode Version");

#else       //ANSI version

#define CF_TCHAR CF_TEXT
TCHAR szDefaultText[] = TEXT("Default Text - ANSI Version");
TCHAR szCaption[]     = TEXT("Clipboard Text Transfers - ANSI Version");

#endif

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR    szAppName[] = TEXT("ClipText");
    HACCEL   hAccel;
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = szAppName;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hInstance     = hInstance;
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
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
    //variable declarations
    static PTSTR pText;
    BOOL         bEnable;
    HGLOBAL      hGlobal;
    HDC          hdc;
    PTSTR        pGlobal;
    PAINTSTRUCT  ps;
    RECT         rect;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            SendMessage(hwnd, WM_COMMAND, IDM_EDIT_RESET, 0);
            return(0);
            
        case WM_INITMENUPOPUP:
            EnableMenuItem(
                    (HMENU)wParam,
                    IDM_EDIT_PASTE,
                    IsClipboardFormatAvailable(CF_TCHAR) ? MF_ENABLED : MF_GRAYED
            );
            
            bEnable = pText ? MF_ENABLED : MF_GRAYED;
            
            EnableMenuItem((HMENU)wParam, IDM_EDIT_CUT,   bEnable);
            EnableMenuItem((HMENU)wParam, IDM_EDIT_COPY,  bEnable);
            EnableMenuItem((HMENU)wParam, IDM_EDIT_CLEAR, bEnable);
            
            break;
        
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_EDIT_PASTE:
                    OpenClipboard(hwnd);
                    
                    if(hGlobal = GetClipboardData(CF_TCHAR))
                    {
                        pGlobal = GlobalLock(hGlobal);
                        if(pText)
                        {
                            free(pText);
                            pText = NULL;
                        }
                        pText = malloc(GlobalSize(hGlobal));
                        lstrcpy(pText, pGlobal);
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    CloseClipboard();
                    return(0);
                    
                case IDM_EDIT_CUT:
                case IDM_EDIT_COPY:
                    if(!pText)
                    {
                        return(0);
                    }
                    
                    
                    OpenClipboard(hwnd);
                    EmptyClipboard();
                    SetClipboardData(CF_TCHAR, NULL);
                    CloseClipboard();
                    
                    if(LOWORD(wParam) == IDM_EDIT_COPY)
                        return(0);
                    
                    //fall through
                case IDM_EDIT_CLEAR:
                    if(pText)
                    {
                        free(pText);
                        pText = NULL;
                    }
                    InvalidateRect(hwnd, NULL, TRUE);
                    return(0);
                    
                case IDM_EDIT_RESET:
                    if(pText)
                    {
                        free(pText);
                        pText = NULL;
                    }
                    pText = malloc( (lstrlen(szDefaultText) + 1) * sizeof(TCHAR));
                    lstrcpy(pText, szDefaultText);
                    InvalidateRect(hwnd, NULL, TRUE);
                    return(0);
            }
            break;
        
        
        case WM_RENDERALLFORMATS:
            OpenClipboard(hwnd);
            EmptyClipboard();
            MessageBox(hwnd, TEXT("WM_RENDERALLFORMATS"), TEXT("Message"), MB_OK|MB_ICONINFORMATION);
                //fall through
                
        case WM_RENDERFORMAT:
            MessageBox(hwnd, TEXT("WM_RENDERFORMAT"), TEXT("Message"), MB_OK|MB_ICONINFORMATION);
            //#define GHND (GMEM_MOVEABLE | GMEM_ZEROINIT)
            hGlobal = GlobalAlloc(
                        GHND | GMEM_SHARE,
                        (lstrlen(pText) + 1) * sizeof(TCHAR)
                    );
            
            pGlobal = GlobalLock(hGlobal);
            lstrcpy(pGlobal, pText);
            GlobalUnlock(hGlobal);
            
            SetClipboardData(CF_TEXT, hGlobal);
            
            if(message == WM_RENDERALLFORMATS)
                CloseClipboard();
            
            return(0);
            
        case WM_DESTROYCLIPBOARD:
            MessageBox(hwnd, TEXT("WM_DESTROYCLIPBOARD"), TEXT("Message"), MB_OK|MB_ICONINFORMATION);
            break;
        
               
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            
                GetClientRect(hwnd, &rect);
                
                if(pText != NULL)
                    DrawText(hdc, pText, -1, &rect, DT_EXPANDTABS | DT_WORDBREAK);
                
            EndPaint(hwnd, &ps);
            return(0);
            
        case WM_DESTROY:
            if(pText)
                free(pText);
            
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}
