/*
    CreatePen() and CreatePenIndirect()
 */

#include <windows.h>

struct
{
    int      iPenStyle;
    int      nWidth;
    COLORREF penColor;
    TCHAR    string[100];
    
} penStruct[] = {
        PS_SOLID,             3,         RGB(  0, 255,   0),   TEXT("PS_SOLID             3         RGB(  0, 255,   0)"),
        PS_DASH,              5,         RGB(255,   0,   0),   TEXT("PS_DASH              5         RGB(255,   0,   0)"), //width greater than 1, consider as PS_SOLID
        PS_DASH,              1,         RGB(255,   0,   0),   TEXT("PS_DASH              1         RGB(255,   0,   0)"),
        PS_DOT,               4,         RGB(  0,   0, 255),   TEXT("PS_DOT               4         RGB(  0,   0, 255)"), //width greater than 1, consider as PS_SOLID
        PS_DOT,               1,         RGB(  0,   0, 255),   TEXT("PS_DOT               1         RGB(  0,   0, 255)"),
        PS_DASHDOT,           3,         RGB(255, 255,   0),   TEXT("PS_DASHDOT           3         RGB(255, 255,   0)"), //width greater than 1, consider as PS_SOLID
        PS_DASHDOT,           1,         RGB(255, 255,   0),   TEXT("PS_DASHDOT           1         RGB(255, 255,   0)"),
        PS_DASHDOTDOT,        3,         RGB(255,   0, 255),   TEXT("PS_DASHDOTDOT        3         RGB(255,   0, 255)"), //width greater than 1, consider as PS_SOLID
        PS_DASHDOTDOT,        1,         RGB(255,   0, 255),   TEXT("PS_DASHDOTDOT        1         RGB(255,   0, 255)"),
        PS_NULL,              3,         RGB(255,   0,   0),   TEXT("PS_NULL              3         RGB(255,   0,   0)"),
        PS_INSIDEFRAME,       6,         RGB(  0, 255, 255),   TEXT("PS_INSIDEFRAME       6         RGB(  0, 255, 255)")
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    TCHAR      szAppName[] = TEXT("PEN");
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
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR);
        return(-1);
    }
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("Pen Styles, width, color"),
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
    HDC         hDC;
    HPEN        hPen;
    PAINTSTRUCT ps;
    TEXTMETRIC  tm;
    int         i;
    static int  cxChar, cyChar;
    
    static TCHAR heading[]   = TEXT("PEN STYLE       PEN WIDTH       PEN COLOR");
    static TCHAR underLine[] = TEXT("_________      ___________     ___________");
    switch(message)
    {
        case WM_CREATE:
            hDC = GetDC(hWnd);
                SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
                GetTextMetrics(hDC, &tm);
            ReleaseDC(hWnd, hDC);
            
            cxChar = tm.tmAveCharWidth;
            cyChar = tm.tmHeight;
            return(0);
    
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
            
                SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
                SetBkMode(hDC, TRANSPARENT);
            
                TextOut(hDC, 0, 0, heading, lstrlen(heading));
                TextOut(hDC, 0, 0, underLine, lstrlen(underLine));
                
                for(i = 0; i < (sizeof(penStruct) / sizeof(penStruct[0])) ; i++)
                {
                    TextOut(hDC, 0, 2 * (i+1) * cyChar, penStruct[i].string, lstrlen(penStruct[i].string));
                    
                    if(i%2 == 1)
                        hPen = CreatePen(penStruct[i].iPenStyle, penStruct[i].nWidth, penStruct[i].penColor);
                    else
                        hPen = CreatePenIndirect((LOGPEN*)&penStruct[i]); /*********/
                        
                    SelectObject(hDC, hPen);
                    
                    MoveToEx(hDC, cxChar * lstrlen(penStruct[i].string) + 50 , 2 * (i + 1) * cyChar, NULL);
                    LineTo  (hDC, cxChar * lstrlen(penStruct[i].string) + 500, 2 * (i + 1) * cyChar);
                    
                    DeleteObject(SelectObject(hDC, GetStockObject(BLACK_PEN)));
                }
                
            EndPaint(hWnd, &ps);
            return(0);
    
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }

    return(DefWindowProc(hWnd, message, wParam, lParam));
}
