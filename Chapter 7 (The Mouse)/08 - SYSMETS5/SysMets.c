/*
 * System Metrics Display Program - Version 4
 */

#include <windows.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    TCHAR      szAppName[] = TEXT("SysMets5");
    HWND       hWnd;
    MSG        msg;
    WNDCLASSEX wndclass;
    
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
                    TEXT("System Metrics Final Version using Mouse wheel for scrolling"),
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
    static int  cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth;
    static int  iDeltaPerLine, iAccumDelta;   //for mouse wheel logic
    HDC         hDC;
    int         i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd;
    PAINTSTRUCT ps;
    SCROLLINFO  si;
    TCHAR       szBuffer[10];
    TEXTMETRIC  tm;
    ULONG       ulScrollLines;   //for mouse wheel logic
    
    switch(message)
    {
        case WM_CREATE:
            hDC = GetDC(hWnd);
            
            GetTextMetrics(hDC, &tm);
            cxChar = tm.tmAveCharWidth;
            
            /*
				The low bit of the tmPitchAndFamily field in the TEXTMETRIC structure is 1 for a variable-width font,
				and 0 for a fixed-pitch font. this program uses this bit to calculate cxCaps from cxChar.
			*/
            cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
            cyChar = tm.tmHeight + tm.tmExternalLeading;
            
            ReleaseDC(hWnd, hDC);
            
                //save the width of the three columns
            iMaxWidth = 40 * cxChar + 22 * cxCaps;
            
            //Fall through for mouse wheel information
        case WM_SETTINGCHANGE:
            SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &ulScrollLines, 0);
            
                //ulScrollLines usually equals 3 or 0 (for no scrolling)
                //WHEEL_DELTA equals 120, so iDeltaPerLine will be 40
                
            if(ulScrollLines)
                iDeltaPerLine = WHEEL_DELTA / ulScrollLines;
            else
                iDeltaPerLine = 0;
            
            return(0);
        
        case WM_SIZE:
        {
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            
                //Set vertical scroll bar range and page size
            si.cbSize = sizeof(si);
            si.fMask  = SIF_RANGE | SIF_PAGE;
            si.nMin   = 0;
            si.nMax   = NUMLINES - 1;
            si.nPage  = cyClient / cyChar;
            SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
            
                //Set horizontal scroll bar range and page size
            si.cbSize = sizeof(si);
            si.fMask  = SIF_RANGE | SIF_PAGE;
            si.nMin   = 0;
            si.nMax   = 2 + iMaxWidth / cxChar;
            si.nPage  = cxClient / cxChar;
            SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
            
            break;
        }
        
        case WM_VSCROLL:
        {
            ///Get all the vertical scroll bar information
            
            si.cbSize = sizeof(si);
            si.fMask  = SIF_ALL;
            GetScrollInfo(hWnd, SB_VERT, &si);
            
                //Save the position for comparison later on
                
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
                    si.nPos -= 1;
                    break;
                    
                case SB_LINEDOWN:
                    si.nPos += 1;
                    break;
                    
                case SB_PAGEUP:
                    si.nPos -= si.nPage;
                    break;
                    
                case SB_PAGEDOWN:
                    si.nPos += si.nPage;
                    break;
                    
                case SB_THUMBTRACK:
                    si.nPos = si.nTrackPos;
                    break;
                    
                default:
                    break;
            }
                
            // Set the position and then retrieve it. Due to adjustments by windows 
            //   it might not be the same as the value set.
            
            si.fMask = SIF_POS;
            SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
            GetScrollInfo(hWnd, SB_VERT, &si);
            
            //If the position has changed, scroll the window and update it
            if(si.nPos != iVertPos)
            {
                ScrollWindow(hWnd, 0, cyChar * (iVertPos - si.nPos), NULL, NULL);
                UpdateWindow(hWnd);
            }
                        
            break;
        }
        
        case WM_HSCROLL:
        {
            //Get all the vertical scroll bar information
            si.cbSize = sizeof(si);
            si.fMask  = SIF_ALL;
            
            //save the position for comparison later on
            GetScrollInfo(hWnd, SB_HORZ, &si);
            iHorzPos = si.nPos;
            
            switch(LOWORD(wParam))
            {
                case SB_LINELEFT:
                    si.nPos -= 1;
                    break;
                    
                case SB_LINERIGHT:
                    si.nPos += 2;
                    break;
                    
                case SB_PAGELEFT:
                    si.nPos -= si.nPage;
                    break;
                    
                case SB_PAGERIGHT:
                    si.nPos += si.nPage;
                    break;
                    
                case SB_THUMBPOSITION:
                    si.nPos = si.nTrackPos;
                    break;
                    
                default:
                    break;
            }
            
            //Set the position and then retrieve it. Due to adjustments by the windows
            // it might not be the same as the value set.
            
            si.fMask = SIF_POS;
            SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
            GetScrollInfo(hWnd, SB_HORZ, &si);
            
            //If the position has changed, scroll the window
            if(si.nPos != iHorzPos)
            {
                ScrollWindow(hWnd, cxChar * (iHorzPos - si.nPos), 0, NULL, NULL);
            }
            
            break;
        }
        
        case WM_KEYDOWN:
        {
            switch(wParam)
            {
                case VK_HOME:
                    SendMessage(hWnd, WM_VSCROLL, SB_TOP, 0);
                    break;
                    
                case VK_END:
                    SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);
                    break;
                    
                case VK_PRIOR: //Page Up
                    SendMessage(hWnd, WM_VSCROLL, SB_PAGEUP, 0);
                    break;
                    
                case VK_NEXT:  //Page Down
                    SendMessage(hWnd, WM_VSCROLL, SB_PAGEDOWN, 0);
                    break;
                    
                case VK_UP:   //UP arrow
                    SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);
                    break;
                    
                case VK_DOWN:  //DOWN arrow
                    SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
                    break;
                    
                case VK_LEFT:  //LEFT arrow
                    SendMessage(hWnd, WM_HSCROLL, SB_PAGEUP, 0);  // SB_PAGEUP == SB_PAGELEFT
                    break;
                    
                case VK_RIGHT: //RIGHT arrow
                    SendMessage(hWnd, WM_HSCROLL, SB_PAGEDOWN, 0);  //SB_PAGEDOWN == SB_PAGERIGHT
                    break;
            }
            
            break;
        }

        case WM_MOUSEWHEEL:
            if(iDeltaPerLine == 0)
                break;
            
            iAccumDelta += (short) HIWORD(wParam); //120 or -120
            
            while(iAccumDelta >= iDeltaPerLine) //120 >= 40
            {
                SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);
                iAccumDelta -= iDeltaPerLine;
            }
            
            while(iAccumDelta <= -iDeltaPerLine) // -120 <= -40
            {
                SendMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
                iAccumDelta += iDeltaPerLine;
            }
            
            return(0);
        
        case WM_PAINT:
        {
            hDC = BeginPaint(hWnd, &ps);
            
                //Get vertical scroll bar position
                si.cbSize = sizeof(si);
                si.fMask  = SIF_POS;
                GetScrollInfo(hWnd, SB_VERT, &si);
                iVertPos = si.nPos;
                
                //Get horizontal scroll bar position
                GetScrollInfo(hWnd, SB_HORZ, &si);
                iHorzPos = si.nPos;
                
                //find painting limits
                iPaintBeg = max(           0, iVertPos + ps.rcPaint.top / cyChar);
                iPaintEnd = min(NUMLINES - 1, iVertPos + ps.rcPaint.bottom / cyChar);
                
                for(i = iPaintBeg; i <= iPaintEnd; i++)
                {
                    x = cxChar * (1 - iHorzPos);
                    y = cyChar * (i - iVertPos);
                    
                    TextOut(hDC, x, y,
                                sysmetrics[i].szLabel,
                                lstrlen(sysmetrics[i].szLabel));
                                
                    TextOut(hDC, x + 22 * cxCaps, y,
                                sysmetrics[i].szDesc,
                                lstrlen(sysmetrics[i].szDesc));
                                
                    SetTextAlign(hDC, TA_RIGHT | TA_TOP);
                    
                    TextOut(hDC, x + 22 * cxCaps + 40 * cxChar, y,
                                szBuffer,
                                wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex))
                            );
                            
                    SetTextAlign(hDC, TA_LEFT | TA_TOP);
                }
            
            EndPaint(hWnd, &ps);
            
            break;
        }
        
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}
