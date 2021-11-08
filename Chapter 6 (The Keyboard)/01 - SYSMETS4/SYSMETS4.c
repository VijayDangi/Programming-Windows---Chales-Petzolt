/*
 * System Metrics Display Program - Version 4
 */

#include <windows.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR      szAppName[] = TEXT("SysMets4");
    HWND       hWnd;
    MSG        msg;
    WNDCLASSEX wndclass;
    
    //code
    wndclass.cbSize        = sizeof (WNDCLASSEX);
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
                    TEXT("System Metrics Version 4"),
                    WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    CW_USEDEFAULT,
                    NULL,
                    NULL,
                    hInstance,
                    NULL
        );
        
    if(hWnd == NULL)
    {
        MessageBox(NULL, TEXT("Error while creating window."), szAppName, MB_ICONERROR);
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
    //variable declarations
    static int  cxChar, cyChar, cxCaps;
    static int  cxClient, cyClient;
    static int  iMaxWidth;
    int         iVertPos, iHorzPos, iPaintBeg, iPaintEnd;
    int         i, x, y;
    TEXTMETRIC  tm;
    HDC         hDC;
    PAINTSTRUCT ps;
    TCHAR       szBuffer[128];
    SCROLLINFO  si;


    //code
    switch(message)
    {
        case WM_CREATE:
            hDC = GetDC(hWnd);
                GetTextMetrics(hDC, &tm);
                cxChar = tm.tmAveCharWidth;
                cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
                cyChar = tm.tmHeight + tm.tmExternalLeading;
            ReleaseDC(hWnd, hDC);
            
                //save width of three columns
            iMaxWidth = 40 * cxChar + 22 * cxCaps;
            break;

        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            
                //set vertical scroll bar range and page size
            si.cbSize = sizeof(SCROLLINFO);
            si.fMask  = SIF_RANGE | SIF_PAGE;
            si.nMin   = 0;
            si.nMax   = NUMLINES;
            si.nPage  = cyClient / cyChar;
            SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
            
                //Set horizontal scroll bar range and page size
            si.cbSize = sizeof(SCROLLINFO);
            si.fMask  = SIF_RANGE | SIF_PAGE;
            si.nMin   = 0;
            si.nMax   = 2 + iMaxWidth / cxChar;
            si.nPage  = cxClient / cxChar;
            SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
            break;

        case WM_VSCROLL:
            //Get all vertical scroll bar information
            
            si.cbSize = sizeof(SCROLLINFO);
            si.fMask  = SIF_ALL;
            GetScrollInfo(hWnd, SB_VERT, &si);
            
                //save the position for comparison later on
            iVertPos = si.nPos;
            
            switch(LOWORD(wParam))
            {
                case SB_TOP:
                    si.nPos = si.nMin;
                    break;
                    
                case SB_BOTTOM:
                    si.nPos = si.nMax;
                    break;
                    
                case SB_LINEUP:
                    si.nPos = si.nPos - 1;
                    break;
                    
                case SB_LINEDOWN:
                    si.nPos = si.nPos + 1;
                    break;
                    
                case SB_PAGEUP:
                    si.nPos = si.nPos - si.nPage;
                    break;
                 
                case SB_PAGEDOWN:
                    si.nPos = si.nPos + si.nPage;
                    break;
                    
                case SB_THUMBTRACK:
                    si.nPos = si.nTrackPos;
                    break;
                    
                default:
                    break;
            }
            
            //Set the position and then retrieve it. Due to adjustments by windows it
            //might not be the same as the value set.
            
            si.fMask = SIF_POS;
            SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
            GetScrollInfo(hWnd, SB_VERT, &si);
            
            if(si.nPos != iVertPos)
            {
                ScrollWindow(hWnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL);
                UpdateWindow(hWnd);
            }
            
            break;

        case WM_HSCROLL:
                //Get all the horizontal scroll bar information
                si.cbSize = sizeof(SCROLLINFO);
                si.fMask  = SIF_ALL;
                
                GetScrollInfo(hWnd, SB_HORZ, &si);
                
                    // save position for comparison
                iHorzPos = si.nPos;
                
                switch(LOWORD(wParam))
                {
                    case SB_LINELEFT:
                        si.nPos = si.nPos - 1;
                        break;
                        
                    case SB_LINERIGHT:
                        si.nPos = si.nPos + 1;
                        break;
                        
                    case SB_PAGELEFT:
                        si.nPos = si.nPos - si.nPage;
                        break;
                        
                    case SB_PAGERIGHT:
                        si.nPos = si.nPos + si.nPage;
                        break;
                        
                    case SB_THUMBPOSITION:
                        si.nPos = si.nTrackPos;
                        break;
                       
                    default:
                        break;
                }
                
                    //Set the position and then retrieve it. Due to adjustments by windows it
                    //might not be the same as the value set.
                si.fMask = SIF_POS;
                SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
                GetScrollInfo(hWnd, SB_HORZ, &si);

                    //if the position has changed, scroll the window
                if(si.nPos != iHorzPos)
                {
                    ScrollWindow(hWnd, cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
                }
                
            break;
        
        case WM_KEYDOWN:
            switch(wParam)
            {
                case VK_HOME:
                    SendMessage(hWnd, WM_VSCROLL, SB_TOP, 0);
                    break;
                 
                case VK_END:
                    SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);
                    break;
                
                case VK_PRIOR:
                    SendMessage(hWnd, WM_VSCROLL, SB_PAGEUP, 0);
                    break;
                    
                case VK_NEXT:
                    SendMessage(hWnd, WM_VSCROLL, SB_PAGEDOWN, 0);
                    break;
                    
                case VK_UP:
                    SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);
                    break;
                    
                case VK_DOWN:
                    SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
                    break;
                    
                case VK_LEFT:
                    SendMessage(hWnd, WM_HSCROLL, SB_LINELEFT, 0);
                    break;
                   
                case VK_RIGHT:
                    SendMessage(hWnd, WM_HSCROLL, SB_LINERIGHT, 0);
                    break;
                
                default:
                    break;
            }
            break;
        
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
            
                    //Get Vertical scroll bar position
                si.cbSize = sizeof(SCROLLINFO);
                si.fMask  = SIF_POS;
                GetScrollInfo(hWnd, SB_VERT, &si);
                iVertPos = si.nPos;
                
                    //Get Horizontal scroll bar position
                GetScrollInfo(hWnd, SB_HORZ, &si);
                iHorzPos = si.nPos;
                
                    //find painting limit
                iPaintBeg = max(0, (iVertPos + ps.rcPaint.top / cyChar));
                iPaintEnd = min(NUMLINES - 1, (iVertPos + ps.rcPaint.bottom / cyChar));
                
                for(i = iPaintBeg; i <= iPaintEnd; i++)
                {
                    x = cxChar * (1 - iHorzPos);
                    y = cyChar * (i - iVertPos);
                    
                    TextOut(hDC,           x, y, sysmetrics[i].szLabel, lstrlen(sysmetrics[i].szLabel));
                    
                    TextOut(hDC, x + 22 * cxCaps, y,  sysmetrics[i].szDesc, lstrlen(sysmetrics[i].szDesc));
                    
                    SetTextAlign(hDC, TA_RIGHT | TA_TOP);
                        TextOut(hDC, x + 22 * cxCaps + 40 * cxChar, y, szBuffer, wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex)));
                    SetTextAlign(hDC, TA_LEFT | TA_TOP);
                }
            EndPaint(hWnd, &ps);
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}
