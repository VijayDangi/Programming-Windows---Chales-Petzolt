/*
 * Timer Demo Program #2
 */

#include <windows.h>
//#include <stdio.h>

#define ID_TIMER  1

LRESULT CALLBACK WndProc   (HWND, UINT, WPARAM, LPARAM);
VOID    CALLBACK TimerProc (HWND, UINT, UINT  , DWORD);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
// int main()
{
    static TCHAR szAppName[] = TEXT("Beeper2");
    HWND         hWnd;
    MSG          msg;
    WNDCLASSEX   wndclass;
    // HINSTANCE    hInstance = GetModuleHandle(NULL);    
    
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
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR);
        return(0);
    }
    
    hWnd = CreateWindow(
            szAppName,
            TEXT("Beeper2 Timer Demo"),
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
        
    // ShowWindow(hWnd, SW_NORMAL);
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
    switch(message)
    {
        case WM_CREATE:
            SetTimer(hWnd, ID_TIMER, 1000, TimerProc);
            return(0);
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}



VOID CALLBACK TimerProc(
        HWND hWnd,           /* Handle to window specified when call to SetTimer */
        UINT message,        /* WM_TIMER */
        UINT iTimerID,       /* Timer ID */
        DWORD dwTime         /* value compatible with the return value from GetTickCount(). This is number of millisecond that has elapsed since Windows has started */
    )
{
    static BOOL fFlipFlop = FALSE;
    HBRUSH      hBrush;
    HDC         hDC;
    RECT        rc;
    
    MessageBeep(MB_ICONWARNING);  //we can use MB_ICON* values also
    fFlipFlop = !fFlipFlop;
    
    GetClientRect(hWnd, &rc);
    
    hDC = GetDC(hWnd);
        hBrush = CreateSolidBrush(fFlipFlop ? RGB(255, 0, 0) : RGB(0, 0, 255));
                
        FillRect(hDC, &rc, hBrush);
        
        DeleteObject(hBrush);
    ReleaseDC(hWnd, hDC);
    
    // printf("%d %d\n", dwTime, GetTickCount());
}