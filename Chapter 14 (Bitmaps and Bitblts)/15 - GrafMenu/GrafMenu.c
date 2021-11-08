/*------------------------------------------------
    GeafMenu.c :- Demonstrates Bitmap Menu Items
------------------------------------------------*/

//Headers
#include <windows.h>
#include "Resource.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
TCHAR szAppName[] = TEXT("GrafMenu");

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    
    //code
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc    = WndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = hInstance;
    wndclass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName   = NULL;
    wndclass.lpszClassName  = szAppName;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("Bitmap Menu Demonstration"),
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
    //function declarations
    void    AddHelpToSys(HINSTANCE, HWND);
    HMENU   CreateMyMenu(HINSTANCE);
    void    DeleteAllBitmaps(HWND);
    
    //variable declarations
    static int iCurrentFont = IDM_FONT_COUR;
    HMENU      hMenu;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            AddHelpToSys( ((LPCREATESTRUCT)lParam)->hInstance, hwnd);
            
            hMenu = CreateMyMenu( ((LPCREATESTRUCT)lParam)->hInstance);
            
            SetMenu(hwnd, hMenu);
            
            CheckMenuItem(hMenu, iCurrentFont, MF_CHECKED);
            return(0);
            
        case WM_SYSCOMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_HELP:
                    MessageBox(hwnd, TEXT("Help not yet implemented1"), szAppName, MB_OK | MB_ICONERROR);
                    return(0);
            }
            break;
            
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_FILE_NEW:
                case IDM_FILE_OPEN:
                case IDM_FILE_SAVE:
                case IDM_FILE_SAVE_AS:
                case IDM_EDIT_UNDO:
                case IDM_EDIT_CUT:
                case IDM_EDIT_COPY:
                case IDM_EDIT_PASTE:
                case IDM_EDIT_CLEAR:
                    MessageBeep(0);
                    return(0);
                    
                case IDM_FONT_COUR:
                case IDM_FONT_ARIAL:
                case IDM_FONT_TIMES:
                    hMenu = GetMenu(hwnd);
                    CheckMenuItem(hMenu, iCurrentFont, MF_UNCHECKED);
                    iCurrentFont = LOWORD(wParam);
                    CheckMenuItem(hMenu, iCurrentFont, MF_CHECKED);
                    return(0);
            }
            break;
            
        case WM_DESTROY:
            DeleteAllBitmaps(hwnd);
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}

/***********************************************************
    AddHelpToSys(): Adds bitmap Help item to System Menu
 ***********************************************************/
void AddHelpToSys(HINSTANCE hInstance, HWND hwnd)
{
    //function declarations
    HBITMAP StretchBitmap(HBITMAP);
    
    //variable declarations
    HBITMAP hBitmap;
    HMENU   hMenu;
    
    //code
    hMenu = GetSystemMenu(hwnd, FALSE);
    hBitmap = StretchBitmap(LoadBitmap(hInstance, TEXT("BitmapHelp")));
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenu, MF_BITMAP, IDM_HELP, (PTSTR)(LONG)hBitmap);
}

/***************************************************
    CreateMyMenu(): Assembles menu from components
 ***************************************************/
HMENU CreateMyMenu(HINSTANCE hInstance)
{
    //function declarations
    HBITMAP GetBitmapFont(int);
    HBITMAP StretchBitmap(HBITMAP);
    
    //variable declarations
    HBITMAP hBitmap;
    HMENU   hMenu, hMenuPopup;
    int     i;
    
    //code
    hMenu = CreateMenu();
    
    hMenuPopup = LoadMenu(hInstance, TEXT("MenuFile"));
    hBitmap = StretchBitmap(LoadBitmap(hInstance, TEXT("BitmapFile")));
    AppendMenu(hMenu, MF_BITMAP | MF_POPUP, (int)hMenuPopup, (PTSTR)(LONG)hBitmap);
    
    hMenuPopup = LoadMenu(hInstance, TEXT("MenuEdit"));
    hBitmap = StretchBitmap(LoadBitmap(hInstance, TEXT("BitmapEdit")));
    AppendMenu(hMenu, MF_BITMAP | MF_POPUP, (int)hMenuPopup, (PTSTR) (LONG) hBitmap);
    
    hMenuPopup = CreateMenu();
    
    for(i = 0; i < 3; i++)
    {
        hBitmap = GetBitmapFont(i);
        AppendMenu(hMenuPopup, MF_BITMAP, IDM_FONT_COUR + i, (PTSTR)(LONG)hBitmap);
    }
    
    hBitmap = StretchBitmap(LoadBitmap(hInstance, TEXT("BitmapFont")));
    AppendMenu(hMenu, MF_BITMAP | MF_POPUP, (int)hMenuPopup, (PTSTR)(LONG)hBitmap);
    
    return(hMenu);
}

/**********************************************************
    StretchBitmap(): Scales bitmap to display resolution
 **********************************************************/
HBITMAP StretchBitmap(HBITMAP hBitmap1)
{
    //variable declarations
    BITMAP  bm1, bm2;
    HBITMAP hBitmap2;
    HDC     hdc, hdcMem1, hdcMem2;
    int     cxChar, cyChar;
    
    //code
        //Get the width and height of a system font character
    cxChar = LOWORD(GetDialogBaseUnits());
    cyChar = HIWORD(GetDialogBaseUnits());
    
        //Create 2 memory DCs compatible with the display
    hdc = CreateIC(TEXT("DISPLAY"), NULL, NULL, NULL);
    hdcMem1 = CreateCompatibleDC(hdc);
    hdcMem2 = CreateCompatibleDC(hdc);
    DeleteDC(hdc);
    
        //Get the dimension of the bitmap to be stretched
    GetObject(hBitmap1, sizeof(BITMAP), (PTSTR)&bm1);
    
        //Scale these dimensions based on the system font size
    bm2 = bm1;
    bm2.bmWidth      = (cxChar * bm2.bmWidth) / 4;
    bm2.bmHeight     = (cxChar * bm2.bmHeight) / 8;
    bm2.bmWidthBytes = ((bm2.bmWidth + 15) / 16) * 2;
    
        //create a new bitmap of larger size
    hBitmap2 = CreateBitmapIndirect(&bm2);
    
        //Select the bitmaps in the memory DCs and do a StretchBlt
    SelectObject(hdcMem1, hBitmap1);
    SelectObject(hdcMem2, hBitmap2);
    
    StretchBlt(hdcMem2, 0, 0, bm2.bmWidth, bm2.bmHeight,
               hdcMem1, 0, 0, bm1.bmWidth, bm1.bmHeight, SRCCOPY);
    
        //clean up
    DeleteDC(hdcMem1);
    DeleteDC(hdcMem2);
    DeleteObject(hBitmap1);
    
    return(hBitmap2);
}

/****************************************************
    GetBitmapFont(): Creates bitmap with font names
 ****************************************************/
HBITMAP GetBitmapFont(int i)
{
    //variable declarations
    static TCHAR *szFaceName[3] = { TEXT("Courier New"), TEXT("Arial"), TEXT("Times New Roman") };
    HBITMAP       hBitmap;
    HDC           hdc, hdcMem;
    HFONT         hFont;
    SIZE          size;
    TEXTMETRIC    tm;
    
    //code
    hdc = CreateIC(TEXT("DISPLAY"), NULL, NULL, NULL);
    GetTextMetrics(hdc, &tm);
    
    hdcMem = CreateCompatibleDC(hdc);
    hFont = CreateFont(2 * tm.tmHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, szFaceName[i]);
    
    hFont = (HFONT) SelectObject(hdcMem, hFont);
    GetTextExtentPoint32(hdcMem, szFaceName[i], lstrlen(szFaceName[i]), &size);
    
    hBitmap = CreateBitmap(size.cx, size.cy, 1, 1, NULL);
    SelectObject(hdcMem, hBitmap);
    
    TextOut(hdcMem, 0, 0, szFaceName[i], lstrlen(szFaceName[i]));
    
    DeleteObject(SelectObject(hdcMem, hFont));
    DeleteDC(hdcMem);
    DeleteDC(hdc);
    
    return(hBitmap);
}

/*************************************************************
    DeleteAllBitmaps(): Deletes all the bitmaps in the menu
 *************************************************************/
void DeleteAllBitmaps(HWND hwnd)
{
    //variable declarations
    HMENU        hMenu;
    int          i;
    MENUITEMINFO mii = { sizeof(MENUITEMINFO), MIIM_SUBMENU | MIIM_TYPE };
    
        //Delete Help bitmap on system menu
    hMenu = GetSystemMenu(hwnd, FALSE);
    GetMenuItemInfo(hMenu, IDM_HELP, FALSE, &mii);
    DeleteObject((HBITMAP)mii.dwTypeData);
    
        //Delete top-level menu bitmaps
    hMenu = GetMenu(hwnd);
    
    for(i = 0; i < 3; i++)
    {
        GetMenuItemInfo(hMenu, i, TRUE, &mii);
        DeleteObject((HBITMAP) mii.dwTypeData);
    }
    
        //Delete bitmap items on font menu
    hMenu = mii.hSubMenu;
    
    for(i = 0; i < 3; i++)
    {
        GetMenuItemInfo(hMenu, i, TRUE, &mii);
        DeleteObject((HBITMAP) mii.dwTypeData);
    }
}

