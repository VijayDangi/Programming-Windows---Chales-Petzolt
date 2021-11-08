/*
 * Mouse button Demo Program
 */

#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("BlockOut");
    HWND         hWnd;
    MSG          msg;
    WNDCLASSEX   wndclass;
    
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
        MessageBox(NULL, TEXT("Error while registering class"), szAppName, MB_OK);
        return(0);
    }
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("Mouse Button Demo #2"),
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
        MessageBox(NULL, TEXT("Error while creating window"), szAppName, MB_OK);
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

void DrawBoxOutline(HWND hWnd, POINT ptBeg, POINT ptEnd)
{
    HDC hDC;
    
    hDC = GetDC(hWnd);
    
        SetROP2(hDC, R2_NOT);
        SelectObject(hDC, GetStockObject(NULL_BRUSH));
        Rectangle(hDC, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
            
    ReleaseDC(hWnd, hDC);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BOOL  fBlocking, fValidBox;
    static POINT ptBeg, ptEnd, ptBoxBeg, ptBoxEnd;
    HDC          hDC;
    PAINTSTRUCT  ps;
    
    switch(message)
    {
        case WM_LBUTTONDOWN:
            ptBeg.x = ptEnd.x = LOWORD(lParam);
            ptBeg.y = ptEnd.y = HIWORD(lParam);
            
            DrawBoxOutline(hWnd, ptBeg, ptEnd);
            
            SetCapture(hWnd);
            SetCursor(LoadCursor(NULL, IDC_CROSS));

            fBlocking = TRUE;
            return(0);
            
        case WM_MOUSEMOVE:
            if(fBlocking)
            {
                // SetCursor(LoadCursor(NULL, IDC_CROSS));
                
                DrawBoxOutline(hWnd, ptBeg, ptEnd);
                
                ptEnd.x = LOWORD(lParam);
                ptEnd.y = HIWORD(lParam);
                
                DrawBoxOutline(hWnd, ptBeg, ptEnd);
            }
            
            return(0);
            
        case WM_LBUTTONUP:
            if(fBlocking)
            {
                DrawBoxOutline(hWnd, ptBeg, ptEnd);
                
                ptBoxBeg   = ptBeg;
                ptBoxEnd.x = LOWORD(lParam);
                ptBoxEnd.y = HIWORD(lParam);
                
                ReleaseCapture();
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                
                fBlocking = FALSE;
                fValidBox = TRUE;
                
                InvalidateRect(hWnd, NULL, TRUE);
            }
            return(0);
           
        case WM_CHAR:
            if(fBlocking & wParam == 27)  //Escape
            {
                DrawBoxOutline(hWnd, ptBeg, ptEnd);
                ReleaseCapture();
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                
                fBlocking = FALSE;
            }
            return(0);
           
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
            
            if(fValidBox)
            {
                SelectObject(hDC, GetStockObject(BLACK_BRUSH));
                Rectangle(hDC, ptBoxBeg.x, ptBoxBeg.y, ptBoxEnd.x, ptBoxEnd.y);
            }
            
            if(fBlocking)
            {
                SetROP2(hDC, R2_NOT);
                SelectObject(hDC, GetStockObject(NULL_BRUSH));
                Rectangle(hDC, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
            }
            
            EndPaint(hWnd, &ps);
            return(0);
           
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}
