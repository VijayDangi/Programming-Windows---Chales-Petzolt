/*---------------------------
    MenuDemo - Menu Demonstration
---------------------------*/

//Header
#include <windows.h>
#include "resource.h"

//macro
#define ID_TIMER 1

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
TCHAR szAppName[] = TEXT("MenuDemo");

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //function declarations
    HMENU CreateOwnMenu();
    
    //variable declarations
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    HMENU    hMenu;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = NULL;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    hMenu = CreateOwnMenu();
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("Menu Demonstration"),
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                NULL,
                hMenu,
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
    static int idColor[5] = {
        WHITE_BRUSH, LTGRAY_BRUSH, GRAY_BRUSH, DKGRAY_BRUSH, BLACK_BRUSH
    };
    static int iSelection = IDM_BKGND_WHITE;
    HMENU      hMenu;
    HMENU      hMenuPopup;
    
    //code
    switch(message)
    {        
        case WM_COMMAND:
            hMenu = GetMenu(hwnd);
            
            switch(LOWORD(wParam))
            {
                case IDM_FILE_NEW:
                case IDM_FILE_OPEN:
                case IDM_FILE_SAVE:
                case IDM_FILE_SAVE_AS:
                    MessageBeep(0);
                    break;
                    
                case IDM_APP_EXIT:
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    break;
                    
                case IDM_EDIT_UNDO:
                case IDM_EDIT_CUT:
                case IDM_EDIT_COPY:
                case IDM_EDIT_PASTE:
                case IDM_EDIT_CLEAR:
                    MessageBeep(0);
                    break;
                    
                    //Note: Login below assumes that IDM_WHITE through IDM_BLACK are consecutive numbers
                    //in the order shown here.
                case IDM_BKGND_WHITE:
                case IDM_BKGND_LTGRAY:
                case IDM_BKGND_GRAY:
                case IDM_BKGND_DKGRAY:
                case IDM_BKGND_BLACK:
                
                    CheckMenuItem(hMenu, iSelection, MF_UNCHECKED);
                    iSelection = LOWORD(wParam);
                    CheckMenuItem(hMenu, iSelection, MF_CHECKED);
                    
                    SetClassLong(hwnd, GCL_HBRBACKGROUND, 
                                    (LONG) GetStockObject(idColor[LOWORD(wParam) - IDM_BKGND_WHITE])
                                );
                                
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                    
                case IDM_TIMER_START:
                    if(SetTimer(hwnd, ID_TIMER, 1000, NULL))
                    {
                        EnableMenuItem(hMenu, IDM_TIMER_START, MF_GRAYED);
                        EnableMenuItem(hMenu, IDM_TIMER_STOP, MF_ENABLED);
                    }
                    break;
                    
                case IDM_TIMER_STOP:
                    KillTimer(hwnd, ID_TIMER);
                    EnableMenuItem(hMenu, IDM_TIMER_START, MF_ENABLED);
                    EnableMenuItem(hMenu, IDM_TIMER_STOP,  MF_GRAYED);
                    break;
                    
                case IDM_APP_HELP:
                    MessageBox(hwnd, TEXT("Help not yet implemented!!"), szAppName, MB_OK | MB_ICONEXCLAMATION);
                    break;
                    
                case IDM_APP_ABOUT:
                    MessageBox(hwnd, TEXT("Menu Demonstration Program!!!\n") TEXT("(c) Charled Petzold, (1998)"),
                                     szAppName, MB_ICONEXCLAMATION | MB_OK);
                    break;
            }
            break;
            
            case WM_TIMER:
                MessageBeep(0);
                break;
                
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
    }
 
    return(DefWindowProc(hwnd, message, wParam, lParam));
}


//CreateOwnMenu()
HMENU CreateOwnMenu()
{
    //variable declarations
    HMENU hMenu;
    HMENU hMenuPopup;
    
    
    //code
    hMenu = CreateMenu();  //Craete Empty Menu
            
    /************ FILE MENU ******************/
    hMenuPopup = CreateMenu();
    
    AppendMenu (hMenuPopup, MF_STRING,    IDM_FILE_NEW,     "&New");
    AppendMenu (hMenuPopup, MF_STRING,    IDM_FILE_OPEN,    "&Open...");
    AppendMenu (hMenuPopup, MF_STRING,    IDM_FILE_SAVE,    "&Save");
    AppendMenu (hMenuPopup, MF_STRING,    IDM_FILE_SAVE_AS, "Save &As...");
    AppendMenu (hMenuPopup, MF_SEPARATOR, 0,                NULL);
    AppendMenu (hMenuPopup, MF_STRING,    IDM_APP_EXIT,     "E&xit");
    
    AppendMenu (hMenu, MF_POPUP, (UINT)hMenuPopup, "&File");  //Add 'File' menu in main menu
    
    /************ EDIT MENU ******************/
    hMenuPopup = CreateMenu(); //create new popup menu
    
    AppendMenu (hMenuPopup, MF_STRING,    IDM_EDIT_UNDO,  "&Undo");
    AppendMenu (hMenuPopup, MF_SEPARATOR, 0,              NULL);
    AppendMenu (hMenuPopup, MF_STRING,    IDM_EDIT_CUT,   "Cu&t");
    AppendMenu (hMenuPopup, MF_STRING,    IDM_EDIT_COPY,  "&Copy");
    AppendMenu (hMenuPopup, MF_STRING,    IDM_EDIT_PASTE, "&Paste");
    AppendMenu (hMenuPopup, MF_STRING,    IDM_EDIT_CLEAR, "De&lete");
   
    AppendMenu (hMenu, MF_POPUP, (UINT)hMenuPopup, "&Edit");  //Add 'Edit' menu in main menu
    
    /************ BACKGROUND MENU ******************/
    hMenuPopup = CreateMenu(); //create new popup menu
    
    AppendMenu (hMenuPopup, MF_STRING | MF_CHECKED, IDM_BKGND_WHITE,  "&White");
    AppendMenu (hMenuPopup, MF_STRING,              IDM_BKGND_LTGRAY, "&Light Gray");
    AppendMenu (hMenuPopup, MF_STRING,              IDM_BKGND_GRAY,   "&Gray");
    AppendMenu (hMenuPopup, MF_STRING,              IDM_BKGND_DKGRAY, "&Dark Gray");
    AppendMenu (hMenuPopup, MF_STRING,              IDM_BKGND_BLACK,  "&Black");
    
    AppendMenu (hMenu, MF_POPUP, (UINT)hMenuPopup, "&Background");   //Add 'Background' menu in main menu
    
    /************ TIMER MENU ******************/
    hMenuPopup = CreateMenu(); //create new popup menu
    
    AppendMenu (hMenuPopup, MF_STRING,             IDM_TIMER_START, "&Start");
    AppendMenu (hMenuPopup, MF_STRING | MF_GRAYED, IDM_TIMER_STOP,  "S&top");
    
    AppendMenu (hMenu, MF_POPUP, (UINT)hMenuPopup, "&Timer"); //Add 'Timer' menu in main menu
    
    /************ HELP MENU ******************/
    hMenuPopup = CreateMenu(); //create new popup menu
    
    AppendMenu (hMenuPopup, MF_STRING, IDM_APP_HELP,  "&Help");
    AppendMenu (hMenuPopup, MF_STRING, IDM_APP_ABOUT, "&About MenuDemo...");
    
    AppendMenu (hMenu, MF_POPUP, (UINT)hMenuPopup, "&Help"); //Add 'Help' menu in main menu

    return(hMenu);
}