/*
 * Mouse Hit-Test Demo Program #2
 *      Simulate Mouse movement using keyboard
 */

#include <windows.h>

#define DIVISIONS 5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    TCHAR      szAppName[] = TEXT("CHECKER");
    HWND       hWnd;
    MSG        msg;
    WNDCLASSEX wndclass;
    
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
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK);
        return(0);
    }
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("Checker #2"),
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
        MessageBox(NULL, TEXT("Error while creating window."), szAppName, MB_OK);
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
    static int  cxBlock, cyBlock;
    static BOOL fState[DIVISIONS][DIVISIONS];
    int         x, y;
    HDC         hDC;
    RECT        rc;
    POINT       pt;
    PAINTSTRUCT ps;
        
    switch(message)
    {
        case WM_CREATE:
            break;
        case WM_SIZE:
            cxBlock = LOWORD(lParam) / DIVISIONS;
            cyBlock = HIWORD(lParam) / DIVISIONS;
            return(0);
        
        case WM_SETFOCUS:
            ShowCursor(TRUE);
            return(0);
            
        case WM_KILLFOCUS:
            ShowCursor(FALSE);
            return(0);
            
        case WM_KEYDOWN:
            GetCursorPos(&pt);
            ScreenToClient(hWnd, &pt);
            
            x = max(0, min(DIVISIONS - 1, pt.x / cxBlock));
            y = max(0, min(DIVISIONS - 1, pt.y / cyBlock));
            
            switch(wParam)
            {
                case VK_UP: //Up Arrow Key
                    y--;
                    break;
                    
                case VK_DOWN: // Down Arrow Key
                    y++;
                    break;
                    
                case VK_LEFT: //Left Arrow Key
                    x--;
                    break;
                    
                case VK_RIGHT: //Right Arrow Key
                    x++;
                    break;
                    
                case VK_HOME: //Home key
                    x = y = 0;
                    break;
                    
                case VK_END: //End Key
                    x = y = DIVISIONS - 1;
                    break;
                    
                case VK_RETURN: //Enter Key
                case VK_SPACE:  //Space Bar
                    SendMessage(
                            hWnd,
                            WM_LBUTTONDOWN,
                            MK_LBUTTON,
                            MAKELONG(x * cxBlock, y * cyBlock)
                        );
                    break;
            }
            
            x = (x + DIVISIONS) % DIVISIONS;
            y = (y + DIVISIONS) % DIVISIONS;
            
            
            pt.x = x * cxBlock + cxBlock / 2;
            pt.y = y * cyBlock + cyBlock / 2;
            
            ClientToScreen(hWnd, &pt);
            SetCursorPos(pt.x, pt.y);
            return(0);
            
        
        case WM_LBUTTONDOWN:
            x = LOWORD(lParam) / cxBlock;
            y = HIWORD(lParam) / cyBlock;
            
            if(x < DIVISIONS && y < DIVISIONS)
            {
                fState[x][y] ^= 1;
                
                rc.left   =  x      * cxBlock;
                rc.top    =  y      * cyBlock;
                rc.right  = (x + 1) * cxBlock;
                rc.bottom = (y + 1) * cyBlock;
                
                InvalidateRect(hWnd, &rc, FALSE);
            }
            else
                MessageBeep(0);
            
            return(0);
            
            
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
            
                for(x = 0; x < DIVISIONS; x++)
                    for(y = 0; y < DIVISIONS; y++)
                    {
                        Rectangle(hDC, x      * cxBlock, y       * cyBlock,
                                      (x + 1) * cxBlock, (y + 1) * cyBlock);
                                      
                        if(fState[x][y])
                        {
                            MoveToEx(hDC,  x      * cxBlock,  y      * cyBlock, NULL);
                            LineTo  (hDC, (x + 1) * cxBlock, (y + 1) * cyBlock);
                            
                            MoveToEx(hDC,  x      * cxBlock, (y + 1) * cyBlock, NULL);
                            LineTo  (hDC, (x + 1) * cxBlock,  y      * cyBlock);
                        }
                    }
            
            EndPaint(hWnd, &ps);
            return(0);
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}
