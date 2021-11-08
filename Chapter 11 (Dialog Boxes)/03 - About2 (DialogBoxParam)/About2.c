/*-----------------------------------------------
    About2.c :- About Box Demo Program No. 2
-----------------------------------------------*/

//Headers
#include <windows.h>
#include "Resource.h"

//global function declarations
LRESULT CALLBACK WndProc     (HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

//global type defination
typedef struct
{
    int iColor;
    int iFigure;
}ABOUTBOX_DATA;


//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR    szAppName[] = TEXT("About2");
    MSG      msg;
    HWND     hwnd;
    WNDCLASS wndclass;
    
    //code
    wndclass.style         = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc   = WndProc;
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = hInstance;
    wndclass.hIcon         = LoadIcon(hInstance, szAppName);
    wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName  = szAppName;
    wndclass.lpszClassName = szAppName;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("About Box Demo Program"),
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
    void PaintWindow(HWND, int, int);
    
    //variable declarations
    static HINSTANCE     hInstance;
    static ABOUTBOX_DATA aboutData = { IDC_BLACK, IDC_RECT };
    PAINTSTRUCT          ps;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
            break;
            
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_APP_ABOUT:
                    if(DialogBoxParam(hInstance, TEXT("AboutBox"), hwnd, AboutDlgProc, (LPARAM)&aboutData))
                    {
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    break;
                    
                default:
                    break;
            }
            break;
            
        case WM_PAINT:
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            
            PaintWindow(hwnd, aboutData.iColor, aboutData.iFigure);
            break;
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}


//AboutDlgProc()
BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    //function declarations
    void PaintTheBlock(HWND, int, int);
    
    //variable declarations
    static HWND hCtrlBlock;
    static int  iColor, iFigure;
    static ABOUTBOX_DATA *pAboutData;
    
    //code
    switch(message)
    {
        case WM_INITDIALOG:
            pAboutData = (ABOUTBOX_DATA*)lParam;
            
            iColor  = pAboutData->iColor;
            iFigure = pAboutData->iFigure;
            
            CheckRadioButton(
                            hDlg,       //Handle to Dialog Box
                            IDC_BLACK,  //Identifier of first radio button in group
                            IDC_WHITE,  //Identifier of last radio button in group
                            iColor      //Identifier of radio button to select
                        );
                        
            CheckRadioButton(hDlg,  IDC_RECT, IDC_ELLIPSE, iFigure);
            
            hCtrlBlock = GetDlgItem(hDlg, IDC_PAINT);
            
            SetFocus(GetDlgItem(hDlg, iColor));
            
            //Set the input focus to a specific control in a dialog box during processing of the WM_INITDIALOG message
            //and return(FALSE) from the message, Windows sets the input focus to the first control in the dialog box
            //that has the WS_TABSTOP style.
            return(FALSE); 
            
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                    pAboutData->iColor = iColor;
                    pAboutData->iFigure = iFigure;
                    EndDialog(hDlg, TRUE);
                    return(TRUE);
                    
                case IDCANCEL:
                    EndDialog(
                        hDlg,   //handle to dialog box
                        FALSE   //value which will become return value of DialogBox()
                     );
                    return(TRUE);
                    
                case IDC_BLACK:
                case IDC_RED:
                case IDC_GREEN:
                case IDC_YELLOW:
                case IDC_BLUE:
                case IDC_MAGENTA:
                case IDC_CYAN:
                case IDC_WHITE:
                    iColor = LOWORD(wParam);
                    CheckRadioButton(hDlg, IDC_BLACK, IDC_WHITE, LOWORD(wParam));
                    PaintTheBlock(hCtrlBlock, iColor, iFigure);
                    return(TRUE);
                    
                case IDC_RECT:
                case IDC_ELLIPSE:
                    iFigure = LOWORD(wParam);
                    CheckRadioButton(hDlg, IDC_RECT, IDC_ELLIPSE, LOWORD(wParam));
                    PaintTheBlock(hCtrlBlock, iColor, iFigure);
                    return(TRUE);
            }
            break;
            
        case WM_PAINT:
            PaintTheBlock(hCtrlBlock, iColor, iFigure);
            break;
    }
    
    return(FALSE);
}


//PaintWindow()
void PaintWindow(HWND hwnd, int iColor, int iFigure)
{
    //variable declarations
    static COLORREF crColor[8] = {
        RGB(  0,   0, 0), RGB(  0,   0, 255),
        RGB(  0, 255, 0), RGB(  0, 255, 255),
        RGB(255,   0, 0), RGB(255,   0, 255),
        RGB(255, 255, 0), RGB(255, 255, 255)
    };
    
    HBRUSH  hBrush;
    HDC     hdc;
    RECT    rect;
    
    //code
    hdc = GetDC(hwnd);

        GetClientRect(hwnd, &rect);
        hBrush = CreateSolidBrush(crColor[iColor - IDC_BLACK]);
        hBrush = (HBRUSH) SelectObject(hdc, hBrush);
        
        if(iFigure == IDC_RECT)
            Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        else
            Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
        
        DeleteObject(SelectObject(hdc, hBrush));

    ReleaseDC(hwnd, hdc);
}


//PaintTheBlock()
void PaintTheBlock(HWND hCtrl, int iColor, int iFigure)
{
    InvalidateRect(hCtrl, NULL, TRUE);
    UpdateWindow(hCtrl);
    PaintWindow(hCtrl, iColor, iFigure);
}
