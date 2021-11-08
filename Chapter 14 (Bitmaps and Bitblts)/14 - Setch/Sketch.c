/*-------------------------------------------------
    Sketch.c - Shadow Bitmap Demonstration
-------------------------------------------------*/

/***
    Left Button => To draw Black line
    Right Button => To draw White line
 ***/

//Header
#include <windows.h>

//global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declaration
    TCHAR     szAppName[] = TEXT("Sketch");
    HWND      hwnd;
    MSG       msg;
    WNDCLASS  wndclass;
    
    //code    
    wndclass.style          = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc    = WndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = hInstance;
    wndclass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszClassName  = szAppName;
    wndclass.lpszMenuName   = NULL;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class"), szAppName, MB_ICONERROR | MB_OK);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("Sketch"),
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
    //function declaration
    void GetLargestDisplaymode(int*, int*);

    //variable declaration
    static BOOL     fLeftButtonDown, fRightButtonDown;
    static HBITMAP  hBitmap;
    static HDC      hdcMem;
    static int      cxBitmap, cyBitmap, cxClient, cyClient, xMouse, yMouse;
    
    HDC             hdc;
    PAINTSTRUCT     ps;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            GetLargestDisplaymode(&cxBitmap, &cyBitmap);
            
            hdc = GetDC(hwnd);
                hBitmap = CreateCompatibleBitmap(hdc, cxBitmap, cyBitmap);
                hdcMem  = CreateCompatibleDC(hdc);
            ReleaseDC(hwnd, hdc);
            
            if(!hBitmap)    //no memory for bitmap
            {
                DeleteDC(hdcMem);
                return(-1);
            }
            
            SelectObject(hdcMem, hBitmap);
            PatBlt(hdcMem, 0, 0, cxBitmap, cyBitmap, WHITENESS);
            
            return(0);
        
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return(0);
            
        case WM_LBUTTONDOWN:
            if(!fRightButtonDown)
                SetCapture(hwnd);
            
            xMouse = LOWORD(lParam);
            yMouse = HIWORD(lParam);
            
            fLeftButtonDown = TRUE;
            return(0);
            
        case WM_LBUTTONUP:
            if(fLeftButtonDown)
                SetCapture(NULL);
            fLeftButtonDown = FALSE;
            return(0);
            
        case WM_RBUTTONDOWN:
            if(!fLeftButtonDown)
                SetCapture(hwnd);
            
            xMouse = LOWORD(lParam);
            yMouse = HIWORD(lParam);
            fRightButtonDown = TRUE;
            return(0);
            
        case WM_RBUTTONUP:
            if(fRightButtonDown)
                SetCapture(NULL);
            
            fRightButtonDown = FALSE;
            return(0);
            
        case WM_MOUSEMOVE:
            if(!fLeftButtonDown && !fRightButtonDown)
                return(0);
            
            hdc = GetDC(hwnd);
            
                SelectObject(   hdc, GetStockObject(fLeftButtonDown ? BLACK_PEN : WHITE_PEN));
                SelectObject(hdcMem, GetStockObject(fLeftButtonDown ? BLACK_PEN : WHITE_PEN));
                
                MoveToEx(   hdc, xMouse, yMouse, NULL);
                MoveToEx(hdcMem, xMouse, yMouse, NULL);
                
                xMouse = (short) LOWORD(lParam);
                yMouse = (short) HIWORD(lParam);
                
                LineTo(   hdc, xMouse, yMouse);
                LineTo(hdcMem, xMouse, yMouse);
        
            ReleaseDC(hwnd, hdc);
            return(0);

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
                BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);
            EndPaint(hwnd, &ps);
            return(0);
            
        case WM_DESTROY:
            DeleteDC(hdcMem);
            DeleteObject(hBitmap);
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}


//GetLargestDisplaymode()
void GetLargestDisplaymode(int *pcxBitmap, int *pcyBitmap)
{
    //variable declaration
    DEVMODE devMode;
    int iModeNum = 0;
    
    //code
    *pcxBitmap = *pcyBitmap = 0;
    
    ZeroMemory(&devMode, sizeof(DEVMODE));
    devMode.dmSize = sizeof(DEVMODE);
    
    while(EnumDisplaySettings(NULL, iModeNum++, &devMode))
    {
        *pcxBitmap = max(*pcxBitmap, (int) devMode.dmPelsWidth);
        *pcyBitmap = max(*pcyBitmap, (int) devMode.dmPelsHeight);
    }
}
