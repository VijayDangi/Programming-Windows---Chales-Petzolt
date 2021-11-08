/*
    Device Capabilities Display Program #1
*/

#include <windows.h>
#include <ctype.h>

#define NUMLINES ((int)(sizeof(devcaps) / sizeof(devcaps[0])))

struct{
    int    iIndex;
    TCHAR *szLabel;
    TCHAR *szDesc;
} devcaps[] = {
    DRIVERVERSION,      TEXT("DRIVERVERSION"),          TEXT("Device Driver Version : "),
    TECHNOLOGY,         TEXT("TECHNOLOGY"),             TEXT("Device Classification : "),
    HORZSIZE,           TEXT("HORZSIZE"),               TEXT("Width in millimeter : "),
    VERTSIZE,           TEXT("VERTSIZE"),               TEXT("Height in millimeter : "),
    HORZRES,            TEXT("HORZRES"),                TEXT("width in pixels : "),
    VERTRES,            TEXT("VERTRES"),                TEXT("width in pixels : "),
    BITSPIXEL,          TEXT("BITSPIXEL"),              TEXT("Color bits per pixel : "),
    PLANES,             TEXT("PLANES"),                 TEXT("Number of planes : "),
    NUMBRUSHES,         TEXT("NUMBRUSHES"),             TEXT("Number of device brushes : "),
    NUMPENS,            TEXT("NUMPENS"),                TEXT("Number of device pens : "),
    NUMMARKERS,         TEXT("NUMMARKERS"),             TEXT("Number of device markers : "),
    NUMFONTS,           TEXT("NUMFONTS"),               TEXT("Number of device fonts : "),
    NUMCOLORS,          TEXT("NUMCOLORS"),              TEXT("Number of device colors : "),
    PDEVICESIZE,        TEXT("PDEVICESIZE"),            TEXT("Size of device structure : "),
    CURVECAPS,          TEXT("CURVECAPS"),              TEXT("Curve Capabilities : "),
    LINECAPS,           TEXT("LINECAPS"),               TEXT("Line Capabilities : "),
    POLYGONALCAPS,      TEXT("POLYGONALCAPS"),          TEXT("Polygonal Capabilities : "),
    TEXTCAPS,           TEXT("TEXTCAPS"),               TEXT("Text Capabilities : "),
    CLIPCAPS,           TEXT("CLIPCAPS"),               TEXT("Clipping Capabilities : "),
    RASTERCAPS,         TEXT("RASTERCAPS"),             TEXT("Bitblt Capabilities : "),
    ASPECTX,            TEXT("ASPECTX"),                TEXT("Relative width of pixels : "),
    ASPECTY,            TEXT("ASPECTY"),                TEXT("Relative height of pixels : "),
    ASPECTXY,           TEXT("ASPECTY"),                TEXT("Relative length of hypotenuse : "),
    LOGPIXELSX,         TEXT("LOGPIXELSX"),             TEXT("Horizontal dots per inch : "),
    LOGPIXELSY,         TEXT("LOGPIXELSY"),             TEXT("Vertical dots per inch : "),
    SIZEPALETTE,        TEXT("SIZEPALETTE"),            TEXT("Number of palette entries : "),
    NUMRESERVED,        TEXT("NUMRESERVED"),            TEXT("Reserved palette entries : "),
    COLORRES,           TEXT("COLORRES"),               TEXT("Actual color resolution : "),
    VREFRESH,           TEXT("VREFRESH"),               TEXT("Current Vertical refresh rate (Hz) : "),
    DESKTOPVERTRES,     TEXT("DESKTOPVERTRES"),         TEXT("Desktop height in pixels : "),
    DESKTOPHORZRES,     TEXT("DESKTOPHORZRES"),         TEXT("Desktop width in pixels : "),
    BLTALIGNMENT  ,     TEXT("BLTALIGNMENT"),           TEXT("Preferred blt alignment : "),
    SHADEBLENDCAPS,     TEXT("SHADEBLENDCAPS"),         TEXT("Shading and blending caps : "),
    COLORMGMTCAPS ,     TEXT("COLORMGMTCAPS"),          TEXT("Color management Caps : ")
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    TCHAR      szAppName[] = TEXT("DevCaps1");
    HWND       hWnd;
    MSG        msg;
    WNDCLASSEX wndclass;
    
    wndclass.cbSize        = sizeof(WNDCLASSEX);
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = NULL;
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
    
    hWnd = CreateWindowEx(
                        WS_EX_CONTEXTHELP,
                        szAppName,
                        TEXT("Device Capabilities #1"),
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
    static int  cxChar, cyChar, cxCaps;
    TCHAR       szBuffer[15];
    HDC         hDC;
    int         i;
    PAINTSTRUCT ps;
    TEXTMETRIC  tm;
    long long   numberOfColors;
        
    switch(message)
    {
        case WM_CREATE:
            hDC = GetDC(hWnd);
                GetTextMetrics(hDC, &tm);
                cxChar = tm.tmAveCharWidth;
                cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
                cyChar = tm.tmHeight;
            ReleaseDC(hWnd, hDC);
            
            return(0);
            
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
            
                for(i = 0; i < NUMLINES; i++)
                {
                    TextOut(hDC, 0, cyChar * i,
                                    devcaps[i].szLabel,
                                    lstrlen(devcaps[i].szLabel));
                                    
                    TextOut(hDC, 14 * cxCaps, cyChar * i,
                                    devcaps[i].szDesc,
                                    lstrlen(devcaps[i].szDesc));
                                    
                    SetTextAlign(hDC, TA_RIGHT | TA_TOP);
                    
                        TextOut(hDC, 14 * cxCaps + 35 * cxChar, cyChar * i,
                                    szBuffer,
                                    wsprintf(szBuffer, TEXT("%5d"), GetDeviceCaps(hDC, devcaps[i].iIndex)));
                                    
                    SetTextAlign(hDC, TA_LEFT | TA_TOP);
                }
                
                // TextOut(hDC, 0, cyChar * (i + 2), TEXT("iColor = 1 << (PLANES * BITSPIXEL);"), lstrlen(TEXT("iColor = 1 << (PLANES * BITSPIXEL);")));
                
                // TextOut(hDC, cxChar * lstrlen(TEXT("iColor = 1 << (PLANES * BITSPIXEL);")) + 10, cyChar * (i + 2), TEXT("Number of colors"), lstrlen(TEXT("Number of colors")));
                
                // SetTextAlign(hDC, TA_RIGHT | TA_TOP);
                    
                        // numberOfColors = 1 << (GetDeviceCaps(hDC, PLANES) * GetDeviceCaps(hDC, BITSPIXEL));

                        // ltoa(numberOfColors, szBuffer, 15);
                        // TextOut(hDC, 14 * cxCaps + 45 * cxChar, cyChar * (i + 2),
                                    // szBuffer,
                                    // lstrlen(szBuffer));
                                    
                // SetTextAlign(hDC, TA_LEFT | TA_TOP);
            
            EndPaint(hWnd, &ps);
            return(0);
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}
