/* ------------------------------
    ENVIRON.c Environment List Box
------------------------------ */

//Header
#include <windows.h>
#include <stdio.h>

#include <signal.h>

//Macro
#define ID_LIST 1
#define ID_TEXT 2

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR      szAppName[] = TEXT("ENVIRON");
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
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    
    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("Environment List Box"),
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
    void FillListBox(HWND);
    
    //variable declarations
    static HWND  hwndList, hwndText;
    int          iIndex, iLength, cxChar, cyChar;
    TCHAR        *pVarName, *pVarValue;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            cxChar = LOWORD(GetDialogBaseUnits());
            cyChar = HIWORD(GetDialogBaseUnits());
            
                //Create listbox and static text windows.
            hwndList = CreateWindow(
                            TEXT("listbox"),
                            NULL,
                            WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_CAPTION | WS_SIZEBOX,
                            cxChar, cyChar * 3,
                            cxChar * 16 + GetSystemMetrics(SM_CXVSCROLL),
                            cyChar * 5,
                            hwnd,
                            (HMENU) ID_LIST,
                            (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                            NULL
                        );
                        
            hwndText = CreateWindow(
                            TEXT("static"),
                            NULL,
                            WS_CHILD | WS_VISIBLE | SS_LEFT,
                            cxChar, cyChar,
                            GetSystemMetrics(SM_CXSCREEN), cyChar,
                            hwnd,
                            (HMENU) ID_TEXT,
                            (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE),
                            NULL
                        );
            // MessageBox(hwnd, TEXT("GOINING"), TEXT("DN"), MB_OK);            
            FillListBox(hwndList);
            // MessageBox(hwnd, TEXT("DONE"), TEXT("DN"), MB_OK);
            break;
            
            
        case WM_SETFOCUS:
            SetFocus(hwndList);
            break;
            
        case WM_COMMAND:
            if(LOWORD(wParam) == ID_LIST && HIWORD(wParam) == LBN_SELCHANGE)
            {
                //Get Current Selection.
                
                iIndex = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
                iLength = SendMessage(hwndList, LB_GETTEXTLEN, iIndex, 0) + 1;
                pVarName = calloc(iLength, sizeof(TCHAR));
                SendMessage(hwndList, LB_GETTEXT, iIndex, (LPARAM) pVarName);
                
                    //Get environment string
                iLength = GetEnvironmentVariable(pVarName, NULL, 0);
                pVarValue = calloc(iLength, sizeof(TCHAR));
                GetEnvironmentVariable(pVarName, pVarValue, iLength);
                
                    //Show it in window
                SetWindowText(hwndText, pVarValue);
                
                free(pVarValue);
                pVarValue = NULL;
                
                free(pVarName);
                pVarName = NULL;
            }
            break;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
        
    return(DefWindowProc(hwnd, message, wParam, lParam));
}


//FillListBox()
void FillListBox(HWND hwndList)
{
    //variable declarations
    int iLength;
    TCHAR *pVarBlock, *pVarBeg, *pVarEnd, *pVarName, *pEnv;
 
    pVarBlock = GetEnvironmentStrings(); //Get pointer to environment block

    pEnv = pVarBlock;
    
    while(*pVarBlock)
    {
        if(*pVarBlock != '=') //Skip variable names beginning with '='
        {
            pVarBeg = pVarBlock;        //Beginning of variable names
            while(*pVarBlock++ != '='); //Scan until '='
            pVarEnd = pVarBlock - 1;    //Position to '=' sign
            iLength = pVarEnd - pVarBeg;    //Length of variable name

                // //Allocate memory for the variable name and terminating zero.
                // //Copy the variable name and append a zero.
                
            pVarName = calloc(iLength + 1, sizeof(TCHAR));
            
                /***
                 *  #define RtlCopyMemory(Destination. Source, Length)  memcpy((Destination), (Source), (Length))    //winnt.h
                 *  #define CopyMemory RtlCopyMemory         //minwinbase.h
                 ***/
            
            CopyMemory(pVarName, pVarBeg, iLength * sizeof(TCHAR));
            pVarName[iLength] = '\0';
           
                // //put the variable name in the list box and free memory
            
            SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM) pVarName);
            free(pVarName);
        }
        while(*pVarBlock++ != '\0');   //scan until terminating zero        
    }   
    FreeEnvironmentStrings(pEnv);
    pEnv = NULL;
    pVarBlock = NULL;
}


