#include <windows.h>

struct
{
    int      iROP2Mode;
    TCHAR    string[20];
    
} ropStruct[] = {
        R2_BLACK,        TEXT("R2_BLACK"),
        R2_NOTMERGEPEN,  TEXT("R2_NOTMERGEPEN"),
        R2_MASKNOTPEN,   TEXT("R2_MASKNOTPEN"),
        R2_NOTCOPYPEN,   TEXT("R2_NOTCOPYPEN"),
        R2_MASKPENNOT,   TEXT("R2_MASKPENNOT"),
        R2_NOT,          TEXT("R2_NOT"),
        R2_XORPEN,       TEXT("R2_XORPEN"),
        R2_NOTMASKPEN,   TEXT("R2_NOTMASKPEN"),
        R2_MASKPEN,      TEXT("R2_MASKPEN"),
        R2_NOTXORPEN,    TEXT("R2_NOTXORPEN"),
        R2_NOP,          TEXT("R2_NOP"),
        R2_MERGENOTPEN,  TEXT("R2_MERGENOTPEN"),
        R2_COPYPEN,      TEXT("R2_COPYPEN"),
        R2_MERGEPENNOT,  TEXT("R2_MERGEPENNOT"),
        R2_MERGEPEN,     TEXT("R2_MERGEPEN"),
        R2_WHITE,        TEXT("R2_WHITE")
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    TCHAR      szAppName[] = TEXT("ROP2");
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
                TEXT("Binary Raster Operation (ROP2)"),
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
    HBRUSH      hBrush;
    PAINTSTRUCT ps;
    TEXTMETRIC  tm;
    int         i;
    RECT        rc, rcTemp;
    static int  cxChar, cyChar;
    
    static TCHAR heading[]   = TEXT("ROP2 MODE");
    static TCHAR underLine[] = TEXT("_________");
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
            
                GetClientRect(hWnd, &rc);
                
                //black
                rcTemp.left = rc.left;
                rcTemp.right = rc.right/2;
                rcTemp.top = rc.top;
                rcTemp.bottom = rc.bottom;
                
                hBrush = CreateSolidBrush(RGB(0, 0, 0));
                FillRect(hDC, &rcTemp, hBrush);                
                DeleteObject(hBrush);
                
                //white
                rcTemp.left = rc.right/2;
                rcTemp.right = rc.right/2 + (rc.right/2)/4;
                rcTemp.top = rc.top;
                rcTemp.bottom = rc.bottom;
                
                hBrush = CreateSolidBrush(RGB(255, 255, 255));
                FillRect(hDC, &rcTemp, hBrush);
                DeleteObject(hBrush);
                
                //red
                rcTemp.left = rc.right/2 + (rc.right/2)/4;
                rcTemp.right = rc.right/2 + 2 * (rc.right/2)/4;
                rcTemp.top = rc.top;
                rcTemp.bottom = rc.bottom;
                
                hBrush = CreateSolidBrush(RGB(255, 0, 0));
                FillRect(hDC, &rcTemp, hBrush);
                DeleteObject(hBrush);
                
                //green
                rcTemp.left = rc.right/2 + 2 * (rc.right/2)/4;
                rcTemp.right = rc.right/2 + 3 * (rc.right/2)/4;
                rcTemp.top = rc.top;
                rcTemp.bottom = rc.bottom;
                
                hBrush = CreateSolidBrush(RGB(0, 255, 0));
                FillRect(hDC, &rcTemp, hBrush);
                DeleteObject(hBrush);
                
                //blue
                rcTemp.left = rc.right/2 + 3 * (rc.right/2)/4;
                rcTemp.right = rc.right/2 + 4 * (rc.right/2)/4;
                rcTemp.top = rc.top;
                rcTemp.bottom = rc.bottom;
                
                hBrush = CreateSolidBrush(RGB(0, 0, 255));
                FillRect(hDC, &rcTemp, hBrush);
                DeleteObject(hBrush);
                
            
                SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
                SetBkMode(hDC, TRANSPARENT);
                SetTextColor(hDC, RGB(0, 255, 0));
            
                TextOut(hDC, 0, 0, heading, lstrlen(heading));
                TextOut(hDC, 0, 0, underLine, lstrlen(underLine));
                
                SelectObject(hDC, CreatePen(PS_SOLID, 10, RGB(0, 0, 0)));
                
                for(i = 0; i < (sizeof(ropStruct) / sizeof(ropStruct[0])) ; i++)
                {
                    TextOut(hDC, 0, 2 * (i+1) * cyChar, ropStruct[i].string, lstrlen(ropStruct[i].string));
                    
                    SetROP2(hDC, ropStruct[i].iROP2Mode);
                    
                    MoveToEx(hDC, 30 * cxChar, 2 * (i + 1) * cyChar, NULL);
                    LineTo  (hDC, rc.right, 2 * (i + 1) * cyChar);
                    
                }
                DeleteObject(SelectObject(hDC, GetStockObject(BLACK_PEN)));
                
            EndPaint(hWnd, &ps);
            return(0);
    
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }

    return(DefWindowProc(hWnd, message, wParam, lParam));
}
