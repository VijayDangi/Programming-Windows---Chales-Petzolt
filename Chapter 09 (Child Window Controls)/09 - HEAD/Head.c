/*----------------------------------------------
    Head.c - Displays beginning (head) of file
----------------------------------------------*/

//Header
#include <windows.h>

//Macro
#define   ID_LIST  1
#define   ID_TEXT  2

#define MAXREAD 8192    //8KB
#define DIRATTR  (DDL_READWRITE | DDL_READONLY | DDL_HIDDEN | DDL_SYSTEM |  \
                  DDL_DIRECTORY | DDL_ARCHIVE  | DDL_DRIVES)
                  
#define DTFLAGS (DT_WORDBREAK | DT_EXPANDTABS | DT_NOCLIP | DT_NOPREFIX)

//global function declarations
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ListProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
WNDPROC OldList;

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR      szAppName[] = TEXT("HEAD");
    HWND       hwnd;
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
    wndclass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    
    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("Head - Display beginning (head) of file (upto 8 KB)"),
                WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                NULL,
                NULL,
                hInstance,
                NULL
            );
            
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return((int)msg.wParam);
}

//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //variable declarations
    static BOOL    bValidFile;
    static BYTE    buffer[MAXREAD];
    static HWND    hwndList, hwndText;
    static RECT    rect;
    static TCHAR   szFile[MAX_PATH + 1];
    HANDLE         hFile;
    HDC            hdc;
    int            i, cxChar, cyChar;
    PAINTSTRUCT    ps;
    TCHAR          szBuffer[MAX_PATH + 1];
    
    //code
    switch(message)
    {
        case WM_CREATE:
            cxChar = LOWORD(GetDialogBaseUnits());
            cyChar = HIWORD(GetDialogBaseUnits());
            
            rect.left = 20 * cxChar;
            rect.top  =  3 * cyChar;
            
            hwndList = CreateWindow(
                        TEXT("listbox"),
                        NULL,
                        WS_CHILDWINDOW | WS_VISIBLE | LBS_STANDARD,   //LBS_STANDARD => LBS_NOTIFY | LBS_SORT | WS_VSCROLL | WS_BORDER;
                        cxChar, 3 * cyChar,
                        cxChar * 13 + GetSystemMetrics(SM_CXVSCROLL),
                        cyChar * 10,
                        hwnd,
                        (HMENU) ID_LIST,
                        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                        NULL
                    );
                    
            GetCurrentDirectory(MAX_PATH + 1, szBuffer);
            
            hwndText = CreateWindow(
                        TEXT("static"),
                        szBuffer,
                        WS_CHILDWINDOW | WS_VISIBLE | SS_LEFT,
                        cxChar, cyChar, cxChar * MAX_PATH, cyChar,
                        hwnd,
                        (HMENU) ID_TEXT,
                        (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                        NULL
                    );
                    
            OldList = (WNDPROC) SetWindowLong(hwndList, GWL_WNDPROC, (LPARAM)ListProc);
            
            SendMessage(hwndList, LB_DIR, DIRATTR, (LPARAM) TEXT("*.*"));
            break;
            
        case WM_SIZE:
            rect.right  = LOWORD(lParam);
            rect.bottom = HIWORD(lParam);
            break;
            
        case WM_SETFOCUS:
            SetFocus(hwndList);
            break;
            
        case WM_COMMAND:
            if(LOWORD(wParam) == ID_LIST && HIWORD(wParam) == LBN_DBLCLK)
            {
                if(LB_ERR == (i = SendMessage(hwndList, LB_GETCURSEL, 0, 0)))
                    break;
                
                SendMessage(hwndList, LB_GETTEXT, i, (LPARAM) szBuffer);  //get string at 'i'th index
                
                if(INVALID_HANDLE_VALUE != (hFile = CreateFile(szBuffer, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL)))
                {
                    //szBuffer is file
                    CloseHandle(hFile);
                    bValidFile = TRUE;
                    lstrcpy(szFile, szBuffer);
                    GetCurrentDirectory(MAX_PATH + 1, szBuffer);
                    
                    if(szBuffer[lstrlen(szBuffer) - 1] != '\\')
                        lstrcat(szBuffer, TEXT("\\"));
                    SetWindowText(hwndText, lstrcat(szBuffer, szFile));
                }
                else
                {
                    //szBuffer is file or drive
                    bValidFile = FALSE;
                    szBuffer[lstrlen(szBuffer) - 1] = '\0';
                    
                        //If setting the directory doesn't work, may be its a drive change, so try that
                    if(!SetCurrentDirectory(szBuffer + 1))
                    {
                        //szBuffer if drive
                        szBuffer[3] = ':';
                        szBuffer[4] = '\0';
                        SetCurrentDirectory(szBuffer + 2);
                    }
                    
                        //Get the new directory name and fill the list box.
                    GetCurrentDirectory(MAX_PATH + 1, szBuffer);
                    SetWindowText(hwndText, szBuffer);
                    SendMessage(hwndList, LB_RESETCONTENT, 0, 0);  //clear/delete all
                    SendMessage(hwndList, LB_DIR, DIRATTR, (LPARAM) TEXT("*.*"));
                }
                
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
            
        case WM_PAINT:
            if(!bValidFile)
                break;
                
            if(INVALID_HANDLE_VALUE == (hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL)))
            {
                bValidFile = FALSE;
                if(hFile)
                {
                    CloseHandle(hFile);
                }
                break;
            }
            
            ReadFile(hFile, buffer, MAXREAD, &i, NULL);
            CloseHandle(hFile);
            
                //'i' now equals the number of bytes in buffer.
                //Commence getting a device context for displaying text.
            
            hdc = BeginPaint(hwnd, &ps);
            
                SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
                SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));
                SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
                
                    //Assume the file is ASCII
                DrawTextA(hdc, buffer, i, &rect, DTFLAGS);
            
            EndPaint(hwnd, &ps);

            break;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}

LRESULT CALLBACK ListProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_KEYDOWN && wParam == VK_RETURN)
        SendMessage(GetParent(hwnd), WM_COMMAND, MAKELONG(1, LBN_DBLCLK), (LPARAM) hwnd);
        
    return(CallWindowProc(OldList, hwnd, message, wParam, lParam));
}
