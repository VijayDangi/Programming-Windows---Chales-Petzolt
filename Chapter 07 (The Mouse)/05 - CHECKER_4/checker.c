/*
 * Mouse Hit-Test Demo Program #3
 *      Child Window and Keyboard messages
 */
#include <windows.h>

#define DIVISIONS 5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

int   idFocus = 0;
TCHAR szChildClass[] = TEXT("Checker4_Child");

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    TCHAR      szAppName[] = TEXT("Checker4");
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
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    
    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK);
        return(0);
    }
    
    wndclass.lpfnWndProc   = ChildWndProc;
    wndclass.cbWndExtra    = sizeof(long);
    wndclass.hIcon         = NULL;
    wndclass.hIconSm       = NULL;
    wndclass.lpszClassName = szChildClass;
    
    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK);
        return(0);
    }
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("Checker #4 Mouse Hit-Test"),
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
        MessageBox(NULL, TEXT("Error Creating window"), szAppName, MB_OK);
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
    static HWND hWndChild[DIVISIONS][DIVISIONS];
    int         cxBlock, cyBlock;
    int         x, y;
    
    switch(message)
    {
        case WM_CREATE:
            for(x = 0; x < DIVISIONS; x++)
                for(y = 0; y < DIVISIONS; y++)
                {
                    hWndChild[x][y] = CreateWindow(
                                            szChildClass,
                                            NULL,
                                            WS_CHILDWINDOW | WS_VISIBLE,
                                            0, 0, 0, 0,
                                            hWnd,
                                            (HMENU) (y << 8 | x),
                                            (HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),
                                            NULL
                                        );
                }
                
            /*
             *  y = 4, x = 2
             *                   y << 8 | x  =>   0000 0100   0000 0010
             *                                   |_________| |_________| 
             *                                        y            x
             */
            return(0);
          
        case WM_SIZE:
            cxBlock = LOWORD(lParam) / DIVISIONS;
            cyBlock = HIWORD(lParam) / DIVISIONS;
            
            for(x = 0; x < DIVISIONS; x++)
                for(y = 0; y < DIVISIONS; y++)
                {
                    MoveWindow(hWndChild[x][y], x * cxBlock, y * cyBlock,
                                                    cxBlock,     cyBlock, TRUE);
                }
            return(0);
            
          
        case WM_LBUTTONDOWN:
            MessageBeep(0);
            return(0);
            
        //On Set-focus message, set focus to child window
        case WM_SETFOCUS:
          SetFocus(GetDlgItem(hWnd, idFocus));
          return(0);
          
        //On Key-down message, possibly change the focus window
        case WM_KEYDOWN:
            x = idFocus & 0xFF;
            y = idFocus >> 8;
            
            switch(wParam)
            {
                case VK_UP:       y--;                        break;
                case VK_DOWN:     y++;                        break;
                case VK_LEFT:     x--;                        break;
                case VK_RIGHT:    x++;                        break;
                case VK_HOME:     x = y = 0;                  break;
                case VK_END:      x = y = DIVISIONS - 1;      break;
                default:          return(0);
            }
            
            x = (x + DIVISIONS) % DIVISIONS;
            y = (y + DIVISIONS) % DIVISIONS;
            
            idFocus = y << 8 | x;
            
            SetFocus(GetDlgItem(hWnd, idFocus));
            return(0);
            
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}


LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC         hDC;
    PAINTSTRUCT ps;
    RECT        rc;
    
    switch(message)
    {
        case WM_CREATE:
            SetWindowLong(hWnd, 0, 0);  //on/off flag
            return(0);
            
        case WM_KEYDOWN:
            //send most key presses to the parent window
            if(wParam != VK_RETURN && wParam != VK_SPACE)
            {
                SendMessage(GetParent(hWnd), message, wParam, lParam);
                return(0);
            }
            
        //fall through
        case WM_LBUTTONDOWN:
            SetWindowLong(hWnd, 0, 1 ^ GetWindowLong(hWnd, 0));
            SetFocus(hWnd);
            InvalidateRect(hWnd, NULL, FALSE);
            return(0);
            
        //for focus message, invalidate the window for repaint
        case WM_SETFOCUS:
            idFocus = GetWindowLong(hWnd, GWL_ID);
           
           //Fall Through
        case WM_KILLFOCUS:
            InvalidateRect(hWnd, NULL, TRUE);
            return(0);
            
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
            
                GetClientRect(hWnd, &rc);
                Rectangle(hDC, 0, 0, rc.right, rc.bottom);
                
                    //Draw the "X" mark
                if(GetWindowLong(hWnd, 0))
                {
                    MoveToEx(hDC,        0,        0, NULL);
                    LineTo  (hDC, rc.right, rc.bottom);
                    
                    MoveToEx(hDC,        0, rc.bottom, NULL);
                    LineTo  (hDC, rc.right,          0);
                }
                    //Draw the "Focus" rectangle
                if(hWnd == GetFocus())
                {
                    rc.left   += rc.right / 10;
                    rc.right  -= rc.left;
                    rc.top    += rc.bottom / 10;
                    rc.bottom -= rc.top;
                    
                    SelectObject(hDC, GetStockObject(NULL_BRUSH));
                    SelectObject(hDC, CreatePen(PS_DASH, 0, 0));
                    
                    Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
                    
                    DeleteObject(SelectObject(hDC, GetStockObject(BLACK_PEN)));
                }
            
            EndPaint(hWnd, &ps);
            return(0);
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}
