/*
 * Typing Program 
 */

#include <windows.h>

#define BUFFER(x,y) *(pBuffer + y * cxBuffer + x)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstnce, PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("Typer");
    HWND         hWnd;
    MSG          msg;
    WNDCLASSEX   wndclass;
    
    wndclass.cbSize        = sizeof(wndclass);
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
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR);
        return(0);
    }
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("Typing Program"),
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
            
    if(!hWnd)
    {
        MessageBox(NULL, TEXT("Error while creating window"), szAppName, MB_ICONERROR);
        return(0);
    }
    
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static DWORD   dwCharSet = DEFAULT_CHARSET;
    static int     cxChar, cyChar, cxClient, cyClient, cxBuffer, cyBuffer, xCaret, yCaret;
    static TCHAR * pBuffer = NULL;
    HDC            hDC;
    int            x, y, i;
    PAINTSTRUCT    ps;
    TEXTMETRIC     tm;
    
    switch(message)
    {
        case WM_INPUTLANGCHANGE:
            dwCharSet = wParam;
            
            //fall through
        case WM_CREATE:
            hDC = GetDC(hWnd);
            
                SelectObject(hDC, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
                GetTextMetrics(hDC, &tm);
                cxChar = tm.tmAveCharWidth;
                cyChar = tm.tmHeight;
                
                DeleteObject(SelectObject(hDC, GetStockObject(SYSTEM_FONT)));
                                
            ReleaseDC(hWnd, hDC);
            
            //fall through
        case WM_SIZE:
            if(message == WM_SIZE)
            {
                cxClient = LOWORD(lParam);
                cyClient = HIWORD(lParam);
            }
            
            //calculate window size in characters
            cxBuffer = max(1, cxClient / cxChar);
            cyBuffer = max(1, cyClient / cyChar);
            
            //allocate memory for buffer and clear it
            if(pBuffer != NULL)
                free(pBuffer);
            
            pBuffer = (TCHAR *) malloc(cxBuffer * cyBuffer * sizeof(TCHAR));
            
            for(y = 0; y < cyBuffer; y++)
                for(x = 0; x < cxBuffer; x++)
                    BUFFER(x, y) = ' ';
                
            //Set caret to upper left corner
            xCaret = 0;
            yCaret = 0;
            
            if(GetFocus() == hWnd)
                SetCaretPos(xCaret * cxChar, yCaret * cyChar);
            
            InvalidateRect(hWnd, NULL, TRUE);
        
        return(0);
        
        case WM_SETFOCUS:
            //create and show caret
            CreateCaret(hWnd, NULL, cxChar, cyChar);
            SetCaretPos(xCaret * cxChar, yCaret * cyChar);
            ShowCaret(hWnd);
        return(0);
            
        
        case WM_KILLFOCUS:
            //hide and destroy the caret
            HideCaret(hWnd);
            DestroyCaret();
        return(0);
        
        case WM_KEYDOWN:
        
            switch(wParam)
            {
                case VK_HOME:
                    xCaret = 0;
                    break;
                    
                case VK_END:
                    xCaret = cxBuffer - 1;
                    break;
                
                case VK_PRIOR: //page up
                    yCaret = 0;
                    break;
                
                case VK_NEXT: //page down
                    yCaret = cyBuffer - 1;
                    break;
                    
                case VK_LEFT:
                    xCaret = max(xCaret - 1, 0);
                    break;
                    
                case VK_RIGHT:
                    xCaret = min(xCaret + 1, cxBuffer - 1);
                    break;
                    
                case VK_UP:
                    yCaret = max(yCaret - 1, 0);
                    break;
                    
                case VK_DOWN:
                    yCaret = min(yCaret + 1, cyBuffer - 1);
                    break;
                    
                case VK_DELETE:
                    for(x = xCaret; x < cxBuffer - 1; x++)
                        BUFFER(x, yCaret) = BUFFER(x + 1, yCaret);
                    
                    BUFFER(cxBuffer - 1, yCaret) = ' ';
                    
                    HideCaret(hWnd);
                    
                    hDC = GetDC(hWnd);
                        SelectObject(hDC, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
                        
                        TextOut(hDC, xCaret * cxChar, yCaret * cyChar, & BUFFER(xCaret, yCaret), cxBuffer - xCaret);
                        
                        DeleteObject(SelectObject(hDC, GetStockObject(SYSTEM_FONT)));
                    ReleaseDC(hWnd, hDC);
                    
                    ShowCaret(hWnd);
                    break;
            }
            SetCaretPos(xCaret * cxChar, yCaret * cyChar);
        
        return(0);
         
         
        case WM_CHAR:
            for(i = 0; i < (int)LOWORD(lParam); i++)
            {
                switch(wParam)
                {
                    case '\b':   //backspace
                        if(xCaret > 0)
                        {
                            xCaret--;
                            SendMessage(hWnd, WM_KEYDOWN, VK_DELETE, 1);
                        }
                        break;
                        
                    case '\t':    //tab
                        do{
                            SendMessage(hWnd, WM_CHAR, ' ', 1);
                        }while(xCaret % 8 != 0);
                        break;
                        
                    case '\n':   //newline or line Feed
                        if(++yCaret == cyBuffer)
                            yCaret = 0;
                        break;
                        
                    case '\r':    //carriage return
                        xCaret = 0;
                        
                        if(++yCaret == cyBuffer)
                            yCaret = 0;
                        break;
                        
                    case '\x1B':   //escape
                        for(y = 0; y < cyBuffer; y++)
                            for(x = 0; x < cxBuffer; x++)
                                BUFFER (x, y) = ' ';
                            
                        xCaret = 0;
                        yCaret = 0;
                        
                        InvalidateRect(hWnd, NULL, FALSE);
                        break;
                        
                    default:
                        BUFFER(xCaret, yCaret) = (TCHAR)wParam;
                        
                        HideCaret(hWnd);
                        
                        hDC = GetDC(hWnd);
                            SelectObject(hDC, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
                        
                            TextOut(hDC, xCaret * cxChar, yCaret * cyChar, & BUFFER(xCaret, yCaret), 1);
                            
                            DeleteObject(SelectObject(hDC, GetStockObject(SYSTEM_FONT)));
                        ReleaseDC(hWnd, hDC);
                        
                        ShowCaret(hWnd);
                        
                        if(++xCaret == cxBuffer)
                        {
                            xCaret = 0;
                            if(++yCaret == cyBuffer)
                                yCaret = 0;
                        }
                        break;
                }
            }
            SetCaretPos(xCaret * cxChar, yCaret * cyChar);
            return(0);
            
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
                SelectObject(hDC, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
                
                for(y = 0; y < cyBuffer; y++)
                    TextOut(hDC, 0, y * cyChar, & BUFFER(0, y), cxBuffer);
                
                DeleteObject(SelectObject(hDC, GetStockObject(SYSTEM_FONT)));
            EndPaint(hWnd, &ps);
        return(0);
         
         
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}
