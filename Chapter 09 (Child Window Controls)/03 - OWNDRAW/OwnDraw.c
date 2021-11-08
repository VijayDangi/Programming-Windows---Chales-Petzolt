/******************
    OWNDRAW.c
******************/

//Headers
#include <windows.h>

//macro
#define ID_SMALLER    1
#define ID_LARGER     2
#define BTN_WIDTH    (8 * cxChar)
#define BTN_HEIGHT   (4 * cyChar)

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations
HINSTANCE ghInst;

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR      szAppName[] = TEXT("OwnDraw");
    HWND       hwnd;
    MSG        msg;
    WNDCLASSEX wndclass;
    
    //code
    ghInst = hInstance;
    
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
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("Owner-Draw Button Demo"),
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
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //function declarations
    void Triangle(HDC, POINT[]);
    
    //variable declarations
    static HWND      hwndSmaller, hwndLarger;
    static int       cxClient, cyClient, cxChar, cyChar;
    int              cx, cy;
    LPDRAWITEMSTRUCT pdis;
    POINT            pt[3];
    RECT             rect;
    
    switch(message)
    {
        case WM_CREATE:
            cxChar = LOWORD(GetDialogBaseUnits());
            cyChar = HIWORD(GetDialogBaseUnits());
            
                //create the owner-draw pushbuttons
            hwndSmaller = CreateWindow(
                                TEXT("BUTTON"), TEXT(""),
                                WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                0, 0, BTN_WIDTH, BTN_HEIGHT,
                                hwnd, (HMENU) ID_SMALLER, ghInst, NULL
                            );
                            
            hwndLarger = CreateWindow(
                                TEXT("BUTTON"), TEXT(""),
                                WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                0, 0, BTN_WIDTH, BTN_HEIGHT,
                                hwnd, (HMENU) ID_LARGER, ghInst, NULL
                            );
                            
            break;
            
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            
                //Move the buttons to the new center
            MoveWindow(hwndSmaller, cxClient/2 - 3 * BTN_WIDTH /2, cyClient/2 -     BTN_HEIGHT /2,
                                    BTN_WIDTH, BTN_HEIGHT, TRUE);
                                    
            MoveWindow(hwndLarger, cxClient/2 +     BTN_WIDTH /2, cyClient/2 -     BTN_HEIGHT /2,
                                    BTN_WIDTH, BTN_HEIGHT, TRUE);
                                    
            break;
            
            
        case WM_COMMAND:
            GetWindowRect(hwnd, &rect);
            
                //Make the window 10% smaller or larger
                
            switch(wParam)
            {
                case ID_SMALLER:
                    rect.left   += cxClient / 20;
                    rect.right  -= cxClient / 20;
                    rect.top    += cyClient / 20;
                    rect.bottom -= cyClient / 20;
                    break;
                    
                 case ID_LARGER:
                    rect.left   -= cxClient / 20;
                    rect.right  += cxClient / 20;
                    rect.top    -= cyClient / 20;
                    rect.bottom += cyClient / 20;
                    break;
            }
            
            MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
            
            break;
            
            
        case WM_DRAWITEM:
            pdis = (LPDRAWITEMSTRUCT) lParam;
            
                //Fill area with white and frame it black
            FillRect(pdis->hDC, &pdis->rcItem, (HBRUSH)GetStockObject(WHITE_BRUSH));
            FrameRect(pdis->hDC, &pdis->rcItem, (HBRUSH) GetStockObject(BLACK_BRUSH));
            
                //Draw inward and outward black triangles
            cx = pdis->rcItem.right  - pdis->rcItem.left;
            cy = pdis->rcItem.bottom - pdis->rcItem.top;
            
            switch(pdis->CtlID)
            {
                case ID_SMALLER:
                    pt[0].x = 3 * cx/8; pt[0].y = 1 * cy/8;
                    pt[1].x = 5 * cx/8; pt[1].y = 1 * cy/8;
                    pt[2].x = 4 * cx/8; pt[2].y = 3 * cy/8;
                    
                    Triangle(pdis->hDC, pt);
                    
                    
                    pt[0].x = 7 * cx/8; pt[0].y = 3 * cy/8;
                    pt[1].x = 7 * cx/8; pt[1].y = 5 * cy/8;
                    pt[2].x = 5 * cx/8; pt[2].y = 4 * cy/8;
                    
                    Triangle(pdis->hDC, pt);
                    
                    
                    pt[0].x = 5 * cx/8; pt[0].y = 7 * cy/8;
                    pt[1].x = 3 * cx/8; pt[1].y = 7 * cy/8;
                    pt[2].x = 4 * cx/8; pt[2].y = 5 * cy/8;
                    
                    Triangle(pdis->hDC, pt);
                    
                    
                    pt[0].x = 1 * cx/8; pt[0].y = 5 * cy/8;
                    pt[1].x = 1 * cx/8; pt[1].y = 3 * cy/8;
                    pt[2].x = 3 * cx/8; pt[2].y = 4 * cy/8;
                    
                    Triangle(pdis->hDC, pt);
                    
                    break;
                    
                    
                case ID_LARGER:
                    pt[0].x = 5 * cx/8; pt[0].y = 3 * cy/8;
                    pt[1].x = 3 * cx/8; pt[1].y = 3 * cy/8;
                    pt[2].x = 4 * cx/8; pt[2].y = 1 * cy/8;
                    
                    Triangle(pdis->hDC, pt);
                    
                    
                    pt[0].x = 5 * cx/8; pt[0].y = 5 * cy/8;
                    pt[1].x = 5 * cx/8; pt[1].y = 3 * cy/8;
                    pt[2].x = 7 * cx/8; pt[2].y = 4 * cy/8;
                    
                    Triangle(pdis->hDC, pt);
                    
                    
                    pt[0].x = 3 * cx/8; pt[0].y = 5 * cy/8;
                    pt[1].x = 5 * cx/8; pt[1].y = 5 * cy/8;
                    pt[2].x = 4 * cx/8; pt[2].y = 7 * cy/8;
                    
                    Triangle(pdis->hDC, pt);
                    
                    
                    pt[0].x = 3 * cx/8; pt[0].y = 3 * cy/8;
                    pt[1].x = 3 * cx/8; pt[1].y = 5 * cy/8;
                    pt[2].x = 1 * cx/8; pt[2].y = 4 * cy/8;
                    
                    Triangle(pdis->hDC, pt);
                    
                    break;
            }
            
                    //Invert the rectangle if the button is selected
            if(pdis->itemState & ODS_SELECTED)
                InvertRect(pdis->hDC, &pdis->rcItem);
                
                //Draw a focus rectangle is the button has the focus
            if(pdis->itemState & ODS_FOCUS)
            {
                pdis->rcItem.left   += cx/16;
                pdis->rcItem.top    += cy/16;
                pdis->rcItem.right  -= cx/16;
                pdis->rcItem.bottom -= cy/16;
                
                DrawFocusRect(pdis->hDC, &pdis->rcItem);
            }
            break;
            
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}

void Triangle(HDC hdc, POINT pt[])
{
    SelectObject(hdc, GetStockObject(BLACK_BRUSH));
    Polygon(hdc, pt, 3);
    SelectObject(hdc, GetStockObject(WHITE_BRUSH));
}
