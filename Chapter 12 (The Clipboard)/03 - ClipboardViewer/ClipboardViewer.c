/*----------------------------------------------
    ClipboardViewer.c :- The Clipboard and Text
----------------------------------------------*/

/*
    To debug:
            cl.exe /EHsc ClipboardViewer.c /D DEBUG /link user32.lib gdi32.lib kernel32.lib
*/

//Headers
#include <windows.h>

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
#ifdef UNICODE  //Unicode version

TCHAR szCaption[]     = TEXT("Simple Clipboard Viewer (Text Only) - Unicode Version");

#else       //ANSI version

TCHAR szCaption[]     = TEXT("Simple Clipboard Viewer (Text Only) - ANSI Version");

#endif

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR    szAppName[] = TEXT("ClipViewer");
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = NULL;
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
    static HWND  hwndNextViewer;
    HGLOBAL      hGlobal;
    HDC          hdc;
    PTSTR        pGlobal;
    PAINTSTRUCT  ps;
    RECT         rect;
    
    //code
    switch(message)
    {
        case WM_CREATE:
        
#ifdef DEBUG
            MessageBox(hwnd, TEXT("SetClipboardViewer()"), TEXT("ClipViewer"), MB_OK | MB_ICONINFORMATION);
#endif

            hwndNextViewer = SetClipboardViewer(hwnd);
            return(0);
            
        case WM_DRAWCLIPBOARD:
#ifdef DEBUG
            MessageBox(hwnd, TEXT("WM_DRAWCLIPBOARD"), TEXT("ClipViewer"), MB_OK | MB_ICONINFORMATION);
#endif
            //Windows send this message to the current clipboard viewer (the most recent window to register itself as a clipboard viewer)
            //whenever the contents of the clipboard change.
            
            if(hwndNextViewer)
            {
                //send this message to the next clipboard viewer.
                /*
                    ---------------------                ---------------------               ---------------------
                    | Current Clipboard | SendMessage()  | Previous Clipboard| SendMessage() | Last Clipboard    |
                    |   Viewer          |--------------->| Viewer (Before    |--  --  --  -->|  Viewer           |
                    | (our application  |                |  our application) |               |                   |
                    ---------------------                ---------------------               ---------------------
                */
                SendMessage(hwndNextViewer, message, wParam, lParam);
            }
            
            InvalidateRect(hwnd, NULL, TRUE);
            return(0);
       
       
        case WM_PAINT:
        
            GetClientRect(hwnd, &rect);
            hdc = BeginPaint(hwnd, &ps);
                
                OpenClipboard(hwnd);
            
            #ifdef UNICODE
                hGlobal = GetClipboardData(CF_UNICODETEXT);
            #else
                hGlobal = GetClipboardData(CF_TEXT);
            #endif
            
                if(hGlobal != NULL)
                {
                    pGlobal = GlobalLock(hGlobal);
                    
                    DrawText(hdc, pGlobal, -1, &rect, DT_EXPANDTABS | DT_WORDBREAK);
                    
                    GlobalUnlock(hGlobal);
                }
            
                CloseClipboard();
                
            EndPaint(hwnd, &ps);
            return(0);
            
            //wParam => window handle to the window removing itself from the chain.
            //lParam => window handle to the next clipboard viewer after the one removing itself from the chain.
        case WM_CHANGECBCHAIN:
            #ifdef DEBUG
             MessageBox(hwnd, TEXT("WM_CHANGECBCHAIN"), TEXT("ClipViewer"), MB_OK | MB_ICONINFORMATION);
            #endif
        
                //if wParam is equal to the value of hwndNextViewer.
                //program must set hwndNextViewer = lParam
                //This action ensure that any futur WM_DRAWCLIPBOARD message you get won't be sent to the window 
                //removing itself from the clipboard viewer chain.
            if((HWND)wParam == hwndNextViewer)
            {
               #ifdef DEBUG
                MessageBox(hwnd, TEXT("(HWND)wParam == hwndNextViewer"), TEXT("ClipViewer"), MB_OK | MB_ICONINFORMATION);
               #endif
                hwndNextViewer = (HWND)lParam;
            }
            
                //if wParam != hwndNextViewer and hwndNextViewer != NULL, send the message to the next clipboard viewer.
            else if(hwndNextViewer)
            {
               #ifdef DEBUG
                MessageBox(hwnd, TEXT("(HWND)wParam != hwndNextViewer"), TEXT("ClipViewer"), MB_OK | MB_ICONINFORMATION);
               #endif
                SendMessage(hwndNextViewer, message, wParam, lParam);
            }
            
            return(0);
        
        case WM_DESTROY:
            
            #ifdef DEBUG
             MessageBox(hwnd, TEXT("ChangeClipboardChain()"), TEXT("ClipViewer"), MB_OK | MB_ICONINFORMATION);
            #endif
            
            ChangeClipboardChain(hwnd, hwndNextViewer); //Windows Sends a WM_CHANGECBCHAIN to the current clipboard viewer.
            
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}



/*

    When Windows first Startup,
        
                        Current clipboard Viewer: NULL
                        
    A Program with window handle of hwnd1 calls SetClipboardViewer(). The function returns NULL,
    which becomes the hwndNextViewer value in that program
    
                       Current clipboard Viewer: hwnd1
                       hwnd1's hwndNextViewer:   NULL
                       
    A second program with window handle of hwnd2, now calls SetClipboardViewer(), and gets back hwnd1.
    
                       Current clipboard Viewer: hwnd2
                       hwnd2's hwndNextViewer:   hwnd1
                       hwnd1's hwndNextViewer:   NULL
                       
    and so on...
                       Current clipboard Viewer: hwnd4
                       hwnd4's hwndNextViewer:   hwnd3
                       hwnd3's hwndNextViewer:   hwnd2
                       hwnd2's hwndNextViewer:   hwnd1
                       hwnd1's hwndNextViewer:   NULL


    
*/
