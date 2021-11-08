/*
    Bezier Curve #2
 */

#include <windows.h>
#include <math.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    TCHAR      szAppName[] = TEXT("Bezier2");
    HWND       hWnd;
    MSG        msg;
    WNDCLASSEX wndclass;
    
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    
    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName,MB_ICONERROR);
        return(-1);
    }
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("Bezier Curve"),
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
        MessageBox(NULL, TEXT("Error creating window."), szAppName,MB_ICONERROR);
        return(-1);
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

void DrawBezier(HDC hDC, POINT apt[])
{
    //PolyBezier(hDC, apt, 4);
    int x, y;
    float fResult;
    POINT pt[101];
    float t;
    float tmp;
    int   i = 0;
    for(t = 0.00f, i = 0; t < 1.0f; t = t + 0.01f)
    {
        tmp = 1 - t;
        fResult =            tmp*tmp*tmp * (float)apt[0].x +
                       3*t * tmp*tmp     * (float)apt[1].x +
                     3*t*t * tmp         * (float)apt[2].x +
                     t*t*t               * (float)apt[3].x;
                     
        pt[i].x = floor(fResult);
        
        fResult =            tmp*tmp*tmp * (float)apt[0].y +
                       3*t * tmp*tmp     * (float)apt[1].y +
                     3*t*t * tmp         * (float)apt[2].y +
                     t*t*t               * (float)apt[3].y;
                     
        pt[i].y = floor(fResult);
        
        i++;
    }
    
    Polyline(hDC, pt, i - 1);
    
    MoveToEx(hDC, apt[0].x, apt[0].y, NULL);
    LineTo  (hDC, apt[1].x, apt[1].y);

    MoveToEx(hDC, apt[2].x, apt[2].y, NULL);
    LineTo  (hDC, apt[3].x, apt[3].y);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINT apt[4];
    HDC          hDC;
    static int   cxClient, cyClient;
    PAINTSTRUCT  ps;
    TEXTMETRIC   tm;
    static int   cxChar, cyChar;
    TCHAR        szBuffer[20];
    int          i;
    
    switch(message)
    {
        case WM_CREATE:
        
            hDC = GetDC(hWnd);
                GetTextMetrics(hDC, &tm);
                cxChar = tm.tmAveCharWidth;
                cyChar = tm.tmHeight;
            ReleaseDC(hWnd, hDC);
            return(0);
            
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            
            apt[0].x = cxClient / 4;
            apt[0].y = cyClient / 2;
            
            apt[1].x = cxClient / 2;
            apt[1].y = cyClient / 4;
            
            apt[2].x =     cxClient / 2;
            apt[2].y = 3 * cyClient / 4;
            
            apt[3].x = 3 * cxClient / 4;
            apt[3].y =     cyClient / 2;
            
            return(0);
        
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MOUSEMOVE:
            if(wParam & MK_LBUTTON || wParam & MK_RBUTTON)
            {
                hDC = GetDC(hWnd);
                    SelectObject(hDC, GetStockObject(WHITE_PEN));
                    DrawBezier(hDC, apt);
                    
                    if((wParam & MK_LBUTTON) && (wParam & MK_CONTROL))
                    {
                        apt[0].x = LOWORD(lParam);
                        apt[0].y = HIWORD(lParam);
                    }
                    else if((wParam & MK_RBUTTON) && (wParam & MK_CONTROL))
                    {
                        apt[3].x = LOWORD(lParam);
                        apt[3].y = HIWORD(lParam);
                    }
                    else if(wParam & MK_LBUTTON)
                    {
                        apt[1].x = LOWORD(lParam);
                        apt[1].y = HIWORD(lParam);
                    }
                    else if(wParam & MK_RBUTTON)
                    {
                        apt[2].x = LOWORD(lParam);
                        apt[2].y = HIWORD(lParam);
                    }
                    
                    SelectObject(hDC, GetStockObject(BLACK_PEN));
                    DrawBezier(hDC, apt);
                    
                    SetTextAlign(hDC, TA_RIGHT | TA_TOP);
                        for(i = 0; i < 4; i++)
                        {
                            TextOut(hDC, cxClient, cyClient -  (5 - i) * cyChar,
                                        szBuffer,
                                        wsprintf(szBuffer, TEXT("x%d = %d, y%d = %d "), i+1, apt[i].x, i+1, apt[i].y));
                        }
                    SetTextAlign(hDC, TA_LEFT | TA_TOP);
                
                ReleaseDC(hWnd, hDC);
            }
            
            return(0);
    
        case WM_PAINT:
            InvalidateRect(hWnd, NULL, TRUE);
            
            hDC = BeginPaint(hWnd, &ps);
                TextOut(hDC, 0, 1 * cyChar, TEXT("MovePoint 1 : Ctrl + Left  Button"), lstrlen(TEXT("MovePoint 1 : Ctrl + Left  Button")));
                TextOut(hDC, 0, 2 * cyChar, TEXT("MovePoint 2 : Left  Button"),        lstrlen(TEXT("MovePoint 2 : Left  Button")));
                TextOut(hDC, 0, 3 * cyChar, TEXT("MovePoint 3 : Right Button"),        lstrlen(TEXT("MovePoint 3 : Right Button")));
                TextOut(hDC, 0, 4 * cyChar, TEXT("MovePoint 4 : Ctrl + Right Button"), lstrlen(TEXT("MovePoint 4 : Ctrl + Right Button")));
                
                SetTextAlign(hDC, TA_RIGHT | TA_TOP);
                    for(i = 0; i < 4; i++)
                    {
                        TextOut(hDC, cxClient, cyClient -  (5 - i) * cyChar,
                                    szBuffer,
                                    wsprintf(szBuffer, TEXT("x%d = %d, y%d = %d "), i+1, apt[i].x, i+1, apt[i].y));
                    }
                SetTextAlign(hDC, TA_LEFT | TA_TOP);
                DrawBezier(hDC, apt);
            EndPaint(hWnd, &ps);
            
            return(0);
    
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }

    return(DefWindowProc(hWnd, message, wParam, lParam));
}

