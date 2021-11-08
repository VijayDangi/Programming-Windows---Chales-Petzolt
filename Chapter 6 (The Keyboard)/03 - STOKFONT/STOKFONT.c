/*
 *  Stock Font Objects
 */

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR      szAppName[] = TEXT("STOCKFONT");
    HWND       hWnd;
    MSG        msg;
    WNDCLASSEX wndclass;
    
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
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR);
        return(0);
    }
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("Stock Fonts"),
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
    static struct
    {
        int     idStockFont;
        TCHAR * szStockFont;
    }
    stockfont[] = {
        OEM_FIXED_FONT,      TEXT("OEM_FIXED_FONT"),
        ANSI_FIXED_FONT,     TEXT("ANSI_FIXED_FONT"),
        ANSI_VAR_FONT,       TEXT("ANSI_VAR_FONT"),
        SYSTEM_FONT,         TEXT("SYSTEM_FONT"),
        DEVICE_DEFAULT_FONT, TEXT("DEVICE_DEFAULT_FONT"),
        SYSTEM_FIXED_FONT,   TEXT("SYSTEM_FIXED_FONT"),
        DEFAULT_GUI_FONT,    TEXT("DEFAULT_GUI_FONT")
    };
    
    static int  iFont, cFonts = sizeof(stockfont)/sizeof(stockfont[0]);
    HDC         hDC;
    int         i, x, y, cxGrid, cyGrid;
    static int  cxClient, cyClient;
    PAINTSTRUCT ps;
    TCHAR       szFaceName[LF_FACESIZE], szBuffer[LF_FACESIZE + 64];
    TEXTMETRIC tm;
    

    //code    
    switch(message)
    {
        case WM_CREATE:
            SetScrollRange(hWnd, SB_VERT, 0, cFonts - 1, TRUE);
        break;
        
        case WM_DISPLAYCHANGE:
            InvalidateRect(hWnd, NULL, TRUE);
        break;
        
        case WM_SIZE:
        {
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            break;
        }
        
        case WM_VSCROLL:
            switch(LOWORD(wParam))
            {
                case SB_TOP:            iFont = 0;                 break;
                case SB_BOTTOM:         iFont = cFonts - 1;        break;
                case SB_LINEUP:
                case SB_PAGEUP:         iFont -= 1;                break;
                case SB_LINEDOWN:
                case SB_PAGEDOWN:       iFont += 1;                break;
                case SB_THUMBPOSITION:  iFont = HIWORD(wParam);    break;
            }
            
            iFont = max(0, min(cFonts - 1, iFont));
            SetScrollPos(hWnd, SB_VERT, iFont, TRUE);
            InvalidateRect(hWnd, NULL, TRUE);
        break;
        
        case WM_KEYDOWN:
            switch(wParam)
            {
                case VK_HOME:       SendMessage(hWnd, WM_VSCROLL, SB_TOP, 0);          break;
                case VK_END:        SendMessage(hWnd, WM_VSCROLL, SB_BOTTOM, 0);       break;
                case VK_PRIOR: //page up
                case VK_LEFT:
                case VK_UP:         SendMessage(hWnd, WM_VSCROLL, SB_LINEUP, 0);       break;
                case VK_NEXT: //Page Down
                case VK_RIGHT:
                case VK_DOWN:       SendMessage(hWnd, WM_VSCROLL, SB_PAGEDOWN, 0);     break;
            }
        break;
        
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
                
                SelectObject(hDC, GetStockObject(stockfont[iFont].idStockFont));
                GetTextFace(hDC, LF_FACESIZE, szFaceName);
                GetTextMetrics(hDC, &tm);
                cxGrid = max(3 * tm.tmAveCharWidth, 2 * tm.tmMaxCharWidth);
                cyGrid = tm.tmHeight + 3;
                
                TextOut(hDC, 0, 0, szBuffer,
                        wsprintf(szBuffer, TEXT(" %s: Face Name = %s, CharSet = %i"),
                                 stockfont[iFont].szStockFont,
                                 szFaceName, tm.tmCharSet
                        )
                     );
                     
                
                    //vertical and horizontal lines
                for(i = 0; i < 17; i++)
                {
                    MoveToEx(hDC, (i + 2) * cxGrid,  2 * cyGrid, NULL);
                    LineTo  (hDC, (i + 2) * cxGrid, 19 * cyGrid);
                    
                    MoveToEx(hDC,      cxGrid, (i + 3) * cyGrid, NULL);
                    LineTo  (hDC, 18 * cxGrid, (i + 3) * cyGrid);
                }
              
              
                SetTextAlign(hDC, TA_TOP | TA_CENTER);
                    //vertical and horizontal headings
                for(i = 0; i < 16; i++)
                {
                    TextOut(hDC, (2 * i + 5) * cxGrid / 2, 2 * cyGrid + 2, szBuffer, wsprintf(szBuffer, TEXT("%X-"), i));
                    
                    TextOut(hDC, 3 * cxGrid /2, (i + 3) * cyGrid + 2, szBuffer, wsprintf(szBuffer, TEXT("-%X"), i));
                }
              
                    //characters
                for(y = 0; y < 16; y++)
                    for(x = 0; x < 16; x++)
                    {
                        TextOut(hDC, (2 * x + 5) * cxGrid / 2,
                                         (y + 3) * cyGrid + 2,
                                         szBuffer,
                                         wsprintf(szBuffer, TEXT("%c"), 16 * x + y)
                            );
                    }
                
            EndPaint(hWnd, &ps);
        break;
        
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}
