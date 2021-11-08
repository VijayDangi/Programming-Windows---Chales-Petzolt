
//Headers
#include <windows.h>

#define IDM_SYS_RESET  1
#define IDM_SYS_REMOVE 2

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
static TCHAR szAppName[] = TEXT("PoorMenu");

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    
    //variable declarations
    HWND     hwnd;
    HMENU    hMenu;
    MSG      msg;
    WNDCLASS wndclass;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = szAppName;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("The Poor-Person's Menu (System Menu)"),
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
    
    hMenu = GetSystemMenu(hwnd, FALSE);
    
    AppendMenu (hMenu, MF_SEPARATOR, 0,              NULL);
    AppendMenu (hMenu, MF_STRING,    IDM_SYS_RESET,  TEXT("Reset System Menu"));
    AppendMenu (hMenu, MF_STRING,    IDM_SYS_REMOVE, TEXT("Remove item from System Menu"));
    
    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return((int)msg.wParam);
}

//WndProc
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //function declarations
    void VJD_RemoveSpecialCharacter(TCHAR*, int);
    
    //variable declarations
    HMENU    hMenu;
    TCHAR    szBuf[28];
    int      iPos = 0;
    int      iCount;
    
    //code
    switch(message)
    {
        case WM_SYSCOMMAND:
            switch(LOWORD(wParam))
            {                    
                case IDM_SYS_REMOVE:
                    hMenu = GetSystemMenu(hwnd, FALSE);
    
                    //GetMenuString() return's number  of character copy excluding null character
                    while(iCount = GetMenuString(hMenu, iPos, szBuf, sizeof(szBuf), MF_BYPOSITION))
                    {
                        // MessageBox(hwnd, szBuf, szAppName, MB_OK);
                        VJD_RemoveSpecialCharacter(szBuf, iCount);
                        // MessageBox(hwnd, szBuf, szAppName, MB_OK);
                        
                        if( 
                            (lstrcmp(szBuf, TEXT("Restore"))  == 0) ||
                            (lstrcmp(szBuf, TEXT("Move"))     == 0) ||
                            (lstrcmp(szBuf, TEXT("Size"))     == 0) ||
                            (lstrcmp(szBuf, TEXT("Minimize")) == 0) ||
                            (lstrcmp(szBuf, TEXT("Maximize")) == 0) ||
                            (lstrcmp(szBuf, TEXT("Close"))    == 0)
                         )
                        {
                            RemoveMenu(hMenu, iPos, MF_BYPOSITION);
                        }
                    }
                    break;
                    
                case IDM_SYS_RESET:
                    GetSystemMenu(hwnd, TRUE); //reset system menu
                    break;
            }
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return(DefWindowProc(hwnd, message, wParam, lParam));
}

//VJD_RemoveSpecialCharacter()
void VJD_RemoveSpecialCharacter(TCHAR* str, int iNumCharacter)
{
    //variable declarations
    int i, j;
    
    //code
    for(i = 0; i < iNumCharacter; i++)
    {
        if(! ( (str[i] >= 65 && str[i] <= 90) || (str[i] >= 97 && str[i] <= 122) ) )
        {
            for(j = i; j < iNumCharacter - 1; j++)
            {
                str[j] = str[j + 1];
            }
            iNumCharacter--;
        }
    }
    str[iNumCharacter] = TEXT('\0');
}
