/*
    Analog Clock Program
 */

#include <windows.h>
#include <math.h>

#define ID_TIMER 1
#define TWOPI    (2 * 3.14159)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //variable declaration
    static TCHAR szAppName[] = TEXT("AnaClock");
    HWND         hWnd;
    MSG          msg;
    WNDCLASSEX   wndclass;
    
    //code
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
                TEXT("ANALOG Clock"),
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
    //function declaration
    void SetIsotropic(HDC, int, int);
    void DrawHands(HDC, SYSTEMTIME*, BOOL);
    void DrawClock(HDC);
    
    //variable declaration
    static int        cxClient, cyClient;
    static SYSTEMTIME stPrevious;
    BOOL              fChange;
    HDC               hDC;
    PAINTSTRUCT       ps;
    SYSTEMTIME        st;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            SetTimer(hWnd, ID_TIMER, 1000, NULL);
            GetLocalTime(&st);
            stPrevious = st;
            return(0);
            
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return(0);
            
        case WM_TIMER:
            GetLocalTime(&st);
            
            fChange = st.wHour   != stPrevious.wHour ||
                      st.wMinute != stPrevious.wMinute;
                      
            hDC = GetDC(hWnd);
            
                SetIsotropic(hDC, cxClient, cyClient);
                
                SelectObject(hDC, GetStockObject(WHITE_PEN));
                DrawHands(hDC, &stPrevious, fChange);
                
                SelectObject(hDC, GetStockObject(BLACK_PEN));
                DrawHands(hDC, &st, TRUE);
            
            ReleaseDC(hWnd, hDC);
            
            stPrevious = st;
            return(0);
        
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
            
                SetIsotropic(hDC, cxClient, cyClient);
                DrawClock(hDC);
                DrawHands(hDC, &stPrevious, TRUE);
            
            EndPaint(hWnd, &ps);
            return(0);
        
        case WM_DESTROY:
            KillTimer(hWnd, ID_TIMER);
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}

void SetIsotropic(HDC hDC, int cxClient, int cyClient)
{
    //code
    SetMapMode(hDC, MM_ISOTROPIC);
    SetWindowExtEx(hDC, 1000, 1000, NULL);
    SetViewportExtEx(hDC, cxClient/2, -cyClient/2, NULL);
    SetViewportOrgEx(hDC, cxClient/2, cyClient/2, NULL);
    // MoveToEx(hDC, 0, 0, NULL);
    // LineTo(hDC, 0, -1000);
}

void DrawClock(HDC hDC)
{
    //function declaration
    void RotatePoint(POINT[], int, int);
    
    //variable declaration
    int   iAngle;
    POINT pt[3];
    
    //code
    for(iAngle = 0; iAngle < 360; iAngle +=6)
    {
        pt[0].x = 0;
        pt[0].y = 900;
        
        RotatePoint(pt, 1, iAngle);
        
        pt[2].x = pt[2].y = iAngle % 5 ? 33 : 100;
        
        pt[0].x -= pt[2].x / 2;
        pt[0].y -= pt[2].y / 2;
        
        pt[1].x = pt[0].x + pt[2].x;
        pt[1].y = pt[0].y + pt[2].y;
        
        SelectObject(hDC, GetStockObject(BLACK_BRUSH));
        
        Ellipse(hDC, pt[0].x, pt[0].y, pt[1].x, pt[1].y);
        // MessageBox(NULL, TEXT(""), TEXT(""), MB_OK);
    }
}

void DrawHands(HDC hDC, SYSTEMTIME *pst, BOOL fChange)
{
    //function declaration
    void RotatePoint(POINT[], int, int);
    
    //variable declaration
    static POINT pt[3][5] = {
                    0, -150, 100,  0,  0, 600, -100, 0,  0, -150,
                    0, -200,  50,  0,  0, 800,  -50, 0,  0, -200,
                    0,    0,   0,  0,  0,   0,    0, 0,  0,  800
                };
                
    int          i, iAngle[3];
    POINT        ptTemp[3][5];
    
    //code
    iAngle[0] = (pst->wHour   * 30) % 360 + pst->wMinute / 2;
    iAngle[1] =  pst->wMinute * 6;
    iAngle[2] =  pst->wSecond * 6;
    
    memcpy(ptTemp, pt, sizeof(pt));
    
    for(i = fChange ? 0 : 2; i < 3; i++)
    {
        RotatePoint(ptTemp[i], 5, iAngle[i]);
        Polyline(hDC, ptTemp[i], 5);
    }
}

void RotatePoint(POINT pt[], int iNum, int iAngle)
{
    //variable declaration
    int   i;
    POINT ptTemp;
    
    //code
    for(i = 0; i < iNum; i++)
    {
        ptTemp.x = (int) (pt[i].x * cos(TWOPI * iAngle / 360) + pt[i].y * sin(TWOPI * iAngle / 360));
        ptTemp.y = (int) (pt[i].y * cos(TWOPI * iAngle / 360) + pt[i].x * sin(TWOPI * iAngle / 360));
        
        pt[i] = ptTemp;
    }
}


