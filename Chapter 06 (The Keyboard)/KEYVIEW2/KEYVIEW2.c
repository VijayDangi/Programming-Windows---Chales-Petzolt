/*
 * Display Keyboard and Character Set
 */

#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variables
FILE *g_fp;
int   g_count;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    TCHAR      szAppName[] = TEXT("KeyView2");
    HWND       hWnd;
    MSG        msg;
    WNDCLASSEX wndclass;

    wndclass.cbSize        = sizeof(wndclass);
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
    
    g_fp = fopen("log.log", "w");
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("Keyboard Message Viewer #2"),
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
        MessageBox(NULL, TEXT("Error while creating window"), szAppName, MB_ICONERROR);
        return(0);
    }
    
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    fclose(g_fp);
    return(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static DWORD dwCharSet = DEFAULT_CHARSET;
    static int   cxClientMax, cyClientMax, cxClient, cyClient, cxChar, cyChar;
    static int   cLinesMax, cLines;
    static PMSG  pMsg;
    static RECT  rectScroll;
    static TCHAR szTop[] = TEXT("Message        Key       Char     ")
                           TEXT("Repeat Scan Ext ALT Prev Tran");
    static TCHAR szUnd[] = TEXT("_______        ___       ____     ")
                           TEXT("______ ____ ___ ___ ____ ____");

    static TCHAR * szFormat[2] = {
        TEXT("%-13s %3d %-15s%c%6u %4d %3s %3s %4s %4s"),
        TEXT("%-13s            0x%04X%1s%c %6u %4d %3s %3s %4s %4s")
    };
    
    static TCHAR * szYes  = TEXT("Yes");
    static TCHAR * szNo   = TEXT("No");
    static TCHAR * szDown = TEXT("Down");
    static TCHAR * szUp   = TEXT("Up");
    
    static TCHAR * szMessage[] = {
        TEXT("WM_KEYDOWN"),     //100
        TEXT("WM_KEYUP"),       //101
        TEXT("WM_CHAR"),        //102
        TEXT("WM_DEADCHAR"),    //103
        TEXT("WM_SYSKEYDOWN"),  //104
        TEXT("WM_SYSKEYUP"),    //105
        TEXT("WM_SYSCHAR"),     //106
        TEXT("WM_SYSDEADCHAR")  //107
    };
    
    HDC         hDC;
    int         i, iType;
    PAINTSTRUCT ps;
    TCHAR       szBuffer[128], szKeyName[32];
    TEXTMETRIC  tm;
    
    
    fprintf(g_fp, "%05d. Message : %05d  0x%08x\n", g_count++, message, message);
    switch(message)
    {
        case WM_INPUTLANGCHANGE:
            dwCharSet = wParam;
                //Fall Through
            
        case WM_CREATE:
        case WM_DISPLAYCHANGE:
            //Get maximum size of client area
            cxClientMax = GetSystemMetrics(SM_CXMAXIMIZED);
            cyClientMax = GetSystemMetrics(SM_CYMAXIMIZED);
            
            //GetCharacter size for fixed-pitch font
            hDC = GetDC(hWnd);
            
                SelectObject(hDC, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
                
                GetTextMetrics(hDC, &tm);
                cxChar = tm.tmAveCharWidth;
                cyChar = tm.tmHeight;
                
                
                DeleteObject(SelectObject(hDC, GetStockObject(SYSTEM_FONT)));
            ReleaseDC(hWnd, hDC);
            
            //Allocate memory for display lines
            if(pMsg)
                free(pMsg);
            
            cLinesMax = cyClientMax / cyChar;
            pMsg = malloc(cLinesMax * sizeof(MSG));
            cLines = 0;
            
                //Fall Trough
        case WM_SIZE:
            if(message == WM_SIZE)
            {
                cxClient = LOWORD(lParam);
                cyClient = HIWORD(lParam);
            }
            
            //Calculate scrolling rectangle
            rectScroll.left   = 0;
            rectScroll.right  = cxClient;
            rectScroll.top    = cyChar;
            rectScroll.bottom = cyChar * (cyClient / cyChar);
            
            InvalidateRect(hWnd, NULL, TRUE);
            
            if(message == WM_INPUTLANGCHANGE)
                return(TRUE);
            
            return(0);
        
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_CHAR:
        case WM_DEADCHAR:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_SYSCHAR:
        case WM_SYSDEADCHAR:
            //Rearrange storage array
            for(i = cLinesMax - 1; i > 0; i--)
                pMsg[i] = pMsg[i - 1];
            
            //store new message
            pMsg[0].hwnd    = hWnd;
            pMsg[0].message = message;
            pMsg[0].wParam  = wParam;
            pMsg[0].lParam  = lParam;
            
            cLines = min(cLines + 1, cLinesMax);
            
                //scroll up the display
            ScrollWindow(
                    hWnd,         //handle to window to scroll
                    0,            //amount of horizontal scrolling  //This parameter must be a negative value to scroll the content of the window to the left. 
                    -cyChar,      //amount of vertical scrolling    //This parameter must be a negative value to scroll the content of the window to the Up. 
                    &rectScroll,  //address of structure with scroll rectangle
                    &rectScroll   //address of structure with clip rectangle
                );
        
        break;
        
        
        case WM_PAINT:
        
            hDC = BeginPaint(hWnd, &ps);
            
                SelectObject(hDC, CreateFont(0, 0, 0, 0, 0, 0, 0, 0, dwCharSet, 0, 0, 0, FIXED_PITCH, NULL));
                
                SetBkMode(hDC, TRANSPARENT);
                TextOut(hDC, 0, 0, szTop, lstrlen(szTop));
                TextOut(hDC, 0, 0, szUnd, lstrlen(szUnd));
                
                for(i = 0; i < min(cLines, cyClient / cyChar - 1); i++)
                {
                    iType = pMsg[i].message == WM_CHAR    ||
                            pMsg[i].message == WM_SYSCHAR ||
                            pMsg[i].message == WM_DEADCHAR||
                            pMsg[i].message == WM_SYSDEADCHAR;
                            
                    GetKeyNameText(pMsg[i].lParam, szKeyName, sizeof(szKeyName) / sizeof(TCHAR));
                    
                    TextOut(hDC,
                            0,
                            (cyClient / cyChar - 1 - i) * cyChar,
                            szBuffer,
                            wsprintf(szBuffer,
                                     szFormat[iType],
                                     szMessage[pMsg[i].message - WM_KEYFIRST], //WM_KEYFIRST  == 100
                                     pMsg[i].wParam,
                                     (PTSTR)(iType ? TEXT(" ") : szKeyName),
                                     (TCHAR) (iType ? pMsg[i].wParam : ' '),
                                     LOWORD(pMsg[i].lParam),
                                     HIWORD(pMsg[i].lParam) & 0xFF,
                                     0x01000000 & pMsg[i].lParam ? szYes  : szNo,
                                     0x20000000 & pMsg[i].lParam ? szYes  : szNo,
                                     0x40000000 & pMsg[i].lParam ? szDown : szUp,
                                     0x80000000 & pMsg[i].lParam ? szUp   : szDown
                            )
                        );
                }
                
                DeleteObject(SelectObject(hDC, GetStockObject(SYSTEM_FONT)));
            
            EndPaint(hWnd, &ps);
        
        return(0);
        
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}
