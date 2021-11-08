/*****************************************
    POPMENU.c - Popup Menu Demonstration (Mirror Main Menu with pop-up menu(ContextMenu)
*****************************************/

/*------------------ REFERENCE ---------------------------
*
* Mirroring Main Menu with TrackPopupMenu().
* 
*   November 2, 1995
*   Microsoft Knowledge Base Article ID: Q99806.
*
-----------------------------------------------*/

//Header
#include <windows.h>
#include "resource.h"

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
HINSTANCE ghInst;
TCHAR     szAppName[] = TEXT("PopMenu");

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName  = szAppName;
    wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.hInstance     = hInstance;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR | MB_OK);
        exit(0);
    }
    
    ghInst = hInstance;
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("Popup Menu Demonstration"),
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
    //variable declarations
    static int   idColor[] = {
            WHITE_BRUSH, LTGRAY_BRUSH, GRAY_BRUSH, DKGRAY_BRUSH, BLACK_BRUSH
    };
    static int   iSelection = IDM_BKGND_WHITE;
    POINT        point;
    
    HMENU        hMenu;
    HMENU        hMenuOurs;
    HMENU        hSubMenu;
    int          nMenu;
    UINT         uMenuState, nID;
    char         szBuf[128];
    
    //code
    switch(message)
    {
        // case WM_CREATE:
        case WM_RBUTTONUP:
            hMenu = GetMenu(hwnd); //The handle to the main menu.
            nMenu = 0; //The index of the menu item
            
            hMenuOurs = CreatePopupMenu();  //Create pop-up menu
            
            //GetMenuState() will return the style of the menu
            //in the LOBYTE of the unsigned int.
            //Return -1 indicates the menu doen not exist, and we
            //have finished creating our pop up.
            while( (uMenuState = GetMenuState(hMenu, nMenu, MF_BYPOSITION)) != -1)
            {
                if(uMenuState != -1)
                {
                    //Get the Menu String
                    GetMenuString(hMenu, nMenu, szBuf, 128, MF_BYPOSITION);
                    if(LOBYTE(uMenuState) & MF_POPUP)   //it's POP-UP menu
                    {
                        hSubMenu = GetSubMenu(hMenu, nMenu);
                        AppendMenu(hMenuOurs, LOBYTE(uMenuState), (UINT)hSubMenu, szBuf);
                    }
                    else
                    {
                        nID = GetMenuItemID(hMenu, nMenu);
                        AppendMenu(hMenuOurs, LOBYTE(uMenuState), nID, szBuf);
                    }
                    nMenu++; //Get the next item
                }
            }
            
            point.x = LOWORD(lParam);
            point.y = HIWORD(lParam);
            
            //TrackPopupMenu() expects screen coordinates
            ClientToScreen(hwnd, &point);
            TrackPopupMenu(
                    hMenuOurs,
                    TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                    point.x,
                    point.y,
                    0,
                    hwnd,
                    NULL);

            //Because we are using parts of the main menu in our
            //pop-up menu, we can't just delete the pop-ip menu, because,
            //that would also delete the main menu.
            //So we must go through the pop-up menu and remove all the items.
            while(RemoveMenu(hMenuOurs, 0, MF_BYPOSITION))
                 ;
             
             //Destroy the pop-up menu.
            DestroyMenu(hMenuOurs);
 /*************************
  * If the menu is never dynamically modified, then the menu
  * hMenuOurs could be made static and created inside the WM_CREATE
  * message, and destroyed in WM_DESTROY message.
  *************************/
            break;
    
            
        case WM_COMMAND:
            hMenu = GetMenu(hwnd);
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
                    break;
                    
                    /**
                        Note: Login below assumes that IDM_BKGND_WHITE through IDM_BKGND_BLACK are consecutive numbers
                        int order show here
                     **/
                case IDM_BKGND_WHITE:
                case IDM_BKGND_LTGRAY:
                case IDM_BKGND_GRAY:
                case IDM_BKGND_DKGRAY:
                case IDM_BKGND_BLACK:
                    CheckMenuItem (hMenu, iSelection, MF_UNCHECKED);
                    iSelection = LOWORD(wParam);
                    CheckMenuItem (hMenu, iSelection, MF_CHECKED);
                    
                    SetClassLong (
                        hwnd,
                        GCL_HBRBACKGROUND,
                        (LONG)GetStockObject(idColor[LOWORD(wParam) - IDM_BKGND_WHITE])
                    );
                    
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                    
                case IDM_APP_ABOUT:
                    MessageBox(hwnd, TEXT("Popup Menu Demonstration Program\n") 
                                     TEXT("(c) Charles Petzold, 1998"), szAppName, MB_ICONEXCLAMATION | MB_OK);
                                     
                    break;
                    
                case IDM_APP_EXIT:
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    break;
                    
                case IDM_APP_HELP:
                    MessageBox(hwnd, TEXT("Help not yet implemented!!"), szAppName, MB_ICONEXCLAMATION | MB_OK);
                    break;
            }
            break;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}
