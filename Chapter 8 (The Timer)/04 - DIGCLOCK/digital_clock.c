/*
 * Digital Clock
 */

#include <windows.h>

#define ID_TIMER 1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //variable declaration
    static TCHAR szAppName[] = TEXT("DigClock");
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
    wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    
    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class"), szAppName, MB_OK);
        return(0);
    }
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("Digital Clock"),
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
    void DisplayTime(HDC, BOOL, BOOL);
    
    //variable declaration
    static BOOL   f24Hour, fSuppress;
    static HBRUSH hBrushGreen;
    static int    cxClient, cyClient;
    HDC           hDC;
    PAINTSTRUCT   ps;
    TCHAR         szBuffer[2];
    XFORM         xForm;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            hBrushGreen = CreateSolidBrush(RGB(0, 255, 0));
            SetTimer(hWnd, ID_TIMER, 1000, NULL);
            
            //fall through
            
        case WM_SETTINGCHANGE:
            GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
            f24Hour = (szBuffer[0] == '1');
            
            GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
            fSuppress = (szBuffer[0] == '0');
            
            InvalidateRect(hWnd, NULL, TRUE);
            return(0);
        
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return(0);
            
        case WM_TIMER:
            InvalidateRect(hWnd, NULL, TRUE);
            return(0);
            
        case WM_PAINT:
            // hDC = GetDC(GetDesktopWindow());
            hDC = BeginPaint(hWnd, &ps);
                        
                // SetGraphicsMode(hDC, GM_ADVANCED);
                SetMapMode(hDC, MM_ISOTROPIC);
                SetWindowExtEx(hDC, 276, 72, NULL);
                SetViewportExtEx(hDC, cxClient, cyClient, NULL);
                
                SetWindowOrgEx(hDC, 138, 36, NULL);
                SetViewportOrgEx(hDC, cxClient / 2, cyClient / 2, NULL);
                
                SelectObject(hDC, GetStockObject(NULL_PEN));
                SelectObject(hDC, hBrushGreen);
                
                // xForm.eM11 = (FLOAT) 1.0;
                // xForm.eM12 = (FLOAT) 1.0;
                // xForm.eM21 = (FLOAT) 0.0;
                // xForm.eM22 = (FLOAT) 1.0;
                // xForm.eDx  = (FLOAT) 0.0;
                // xForm.eDy  = (FLOAT) 0.0;
                
                // SetWorldTransform(hDC, &xForm);    
                                
                DisplayTime(hDC, f24Hour, fSuppress);
                
                
            // ReleaseDC(GetDesktopWindow(), hDC);              
            EndPaint(hWnd, &ps);
            return(0);                
        
        case WM_DESTROY:
            KillTimer(hWnd, ID_TIMER);
            DeleteObject(hBrushGreen);
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}


void DisplayTime(HDC hDC, BOOL f24Hour, BOOL fSuppress)
{
    //function declaration
    void DisplayTwoDigits(HDC, int, BOOL);
    void DisplayColon(HDC);
    
    //variable declaration
    SYSTEMTIME st;
    
    //code
    GetLocalTime(&st);
    
    if(f24Hour)
        DisplayTwoDigits(hDC, st.wHour, fSuppress);
    else
        DisplayTwoDigits(hDC, (st.wHour %= 12) ? st.wHour : 12, fSuppress);
    
    DisplayColon(hDC);
    DisplayTwoDigits(hDC, st.wMinute, FALSE);
    DisplayColon(hDC);
    DisplayTwoDigits(hDC, st.wSecond, FALSE);
}


void DisplayTwoDigits(HDC hDC, int iNumber, BOOL fSuppress)
{
    //function declaration
    void DisplayDigit(HDC, int);
    
    //code
    if(!fSuppress || (iNumber / 10 != 0))
        DisplayDigit(hDC, iNumber / 10);
    
    OffsetWindowOrgEx(hDC, -42, 0, NULL);
    DisplayDigit(hDC, iNumber % 10);
    OffsetWindowOrgEx(hDC, -42, 0, NULL);
}

void DisplayDigit(HDC hDC, int iNumber)
{
    //variable declaration
    
            /*
                       ___
                      | 1 |
                  2   |___|  3
                      | 4 |
                  5   |___|  6
                        7
                        
                   SEVEN SEGMENT LED
             */
    static BOOL  fSevenSegment[10][7] = {
                        {1, 1, 1, 0, 1, 1, 1},  //0
                        {0, 0, 1, 0, 0, 1, 0},  //1
                        {1, 0, 1, 1, 1, 0, 1},  //2
                        {1, 0, 1, 1, 0, 1, 1},  //3
                        {0, 1, 1, 1, 0, 1, 0},  //4
                        {1, 1, 0, 1, 0, 1, 1},  //5
                        {1, 1, 0, 1, 1, 1, 1},  //6
                        {1, 0, 1, 0, 0, 1, 0},  //7
                        {1, 1, 1, 1, 1, 1, 1},  //8
                        {1, 1, 1, 1, 0, 1, 1}   //9
                    };
                    /*
                              ___________________
                             /                   \
                          / \\___________________// \
                         |   |                   |   |
                         |   |                   |   |
                         |   |                   |   |
                         |   |                   |   |
                         |   |                   |   |
                         |   |                   |   |
                         |   |                   |   |
                         |   |___________________|   |
                          \ //                   \\ /
                          / \\___________________// \
                         |   |                   |   |
                         |   |                   |   |
                         |   |                   |   |
                         |   |                   |   |
                         |   |                   |   |
                         |   |                   |   |
                         |   |                   |   |
                         |   |___________________|   |
                          \ //                   \\ /
                             \___________________/
                             
                             
                    */
    static POINT ptSegment[7][6] = {
                        {  { 7,  6}, {11,  2}, {31,  2}, {35,  6}, {31, 10}, {11, 10} }, //upper-horizontal
                        {  { 6,  7}, {10, 11}, {10, 31}, { 6, 35}, { 2, 31}, { 2, 11} }, //
                        {  {36,  7}, {40, 11}, {40, 31}, {36, 35}, {32, 31}, {32, 11} },
                        {  { 7, 36}, {11, 32}, {31, 32}, {35, 36}, {31, 40}, {11, 40} },
                        {  { 6, 37}, {10, 41}, {10, 61}, { 6, 65}, { 2, 61}, { 2, 41} },
                        {  {36, 37}, {40, 41}, {40, 61}, {36, 65}, {32, 61}, {32, 41} },
                        {  { 7, 66}, {11, 62}, {31, 62}, {35, 66}, {31, 70}, {11, 70} }
                    };
                    
    int          iSeg;
    
    
    for(iSeg = 0; iSeg < 7; iSeg++)
        if(fSevenSegment[iNumber][iSeg])
            Polygon(hDC, ptSegment[iSeg], 6);
}

void DisplayColon(HDC hDC)
{
                            /*
                                    /\
                                    \/
                                    
                                    
                                    /\
                                    \/
                            */
    POINT ptColon [2][4] = {
                    { 2, 21, 6, 17, 10, 21, 6, 25},
                    { 2, 51, 6, 47, 10, 51, 6, 55}
                };
                
    Polygon(hDC, ptColon[0], 4);
    Polygon(hDC, ptColon[1], 4);
    
    OffsetWindowOrgEx(hDC, -12, 0, NULL);
}
