/*
 * Mouse Hit-Test Demo Program #3
 *      Child Window
 */
#include <windows.h>

#define DIVISIONS 5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR szChildClass[] = TEXT("Checker3_Child");

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
                TEXT("Checker #3 Mouse Hit-Test"),
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
    static HWND  hWndChild[DIVISIONS] [DIVISIONS];
    int          cxBlock, cyBlock;
    int          x, y;
    
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
                                            0,
                                            0,
                                            0,
                                            0,
                                            hWnd,
                                            (HMENU)( y << 8 | x),
                                            (HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE),
                                            NULL
                                        );
                }
            return(0);
            
        case WM_SIZE:
            cxBlock = LOWORD(lParam) / DIVISIONS;
            cyBlock = HIWORD(lParam) / DIVISIONS;
            
            for(x = 0; x < DIVISIONS; x++)
                for(y = 0; y < DIVISIONS; y++)
                {
                    MoveWindow(hWndChild[x][y],
                                x * cxBlock, y * cyBlock,
                                    cxBlock,     cyBlock,  TRUE);
                }
                
            return(0);
            
        
        case WM_LBUTTONDOWN:
            MessageBeep(0);
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
            SetWindowLong(hWnd, 0, 0);   //on/off flag
            return(0);
            
        case WM_LBUTTONDOWN:
            SetWindowLong(hWnd, 0, 1 ^ GetWindowLong(hWnd, 0));
            InvalidateRect(hWnd, NULL, FALSE);
            return(0);
            
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
            
                GetClientRect(hWnd, &rc);
                Rectangle(hDC, 0, 0, rc.right, rc.bottom);
                
                if(GetWindowLong(hWnd, 0))
                {
                    MoveToEx(hDC, 0,                0, NULL);
                    LineTo  (hDC, rc.right, rc.bottom);
                    
                    MoveToEx(hDC, 0,        rc.bottom, NULL);
                    LineTo  (hDC, rc.right,         0);
                }
            
            EndPaint(hWnd, &ps);
            return(0);
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}

