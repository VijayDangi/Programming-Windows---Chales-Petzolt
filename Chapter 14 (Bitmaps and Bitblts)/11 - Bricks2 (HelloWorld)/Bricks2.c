/*------------------------------------------
    BRICKS2.c - CreateBitmap Demonstration
------------------------------------------*/

//Header
#include <windows.h>

//global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declaration
    TCHAR     szAppName[] = TEXT("Bricks2");
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
    wndclass.hbrBackground  = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndclass.lpszClassName  = szAppName;
    wndclass.lpszMenuName   = NULL;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class"), szAppName, MB_ICONERROR | MB_OK);
        exit(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("CreateBitmap Demo"),
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

/*
        0 1 0 1  0 0 0 1  0 1 1 1  0 1 1 1  0 0 0 1  = 0x51 0x77 0x1
        0 1 0 1  0 1 1 1  0 1 1 1  0 1 1 1  0 1 0 1  = 0x57 0x77 0x5
        0 0 0 1  0 0 1 1  0 1 1 1  0 1 1 1  0 1 0 1  = 0x13 0x77 0x5
        0 1 0 1  0 1 1 1  0 1 1 1  0 1 1 1  0 1 0 1  = 0x57 0x77 0x5
        0 1 0 1  0 0 0 1  0 0 0 1  0 0 0 1  0 0 0 1  = 0x51 0x11 0x1
 */

//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //variable declaration
    // static BITMAP bitmap = {
                         // 0,     //bmType   => set to 0
                        // 20,     //bmWidth  => width in pixels
                         // 5,     //bmHeight => height in pixels
                         // 4,     //bmWidthyBytes => width of row in bytes (each row of pixels has an even number of bytes)
                         // 1,     //bmPlanes => number of color planes
                         // 1      //bmBitsPixel => number of bits per pixel
               // };
               
    // static BYTE bits[5][4] = {
        // 0x51, 0x77, 0x10, 0x00,
        // 0x57, 0x77, 0x50, 0x00,
        // 0x13, 0x77, 0x50, 0x00,
        // 0x57, 0x77, 0x50, 0x00,
        // 0x51, 0x11, 0x10, 0x00,
    // };
    
    
    
    static BITMAP bitmap = {
                         0,     //bmType   => set to 0
                        32,     //bmWidth  => width in pixels
                         9,     //bmHeight => height in pixels
                         4,     //bmWidthyBytes => width of row in bytes (each row of pixels has an even number of bytes)
                         1,     //bmPlanes => number of color planes
                         1      //bmBitsPixel => number of bits per pixel
               };
               
    static BYTE bits[9][4] = {
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        
        0x51, 0x77, 0x1F, 0xFF,
        0x57, 0x77, 0x5F, 0xFF,
        0x13, 0x77, 0x5F, 0xFF,
        0x57, 0x77, 0x5F, 0xFF,
        0x51, 0x11, 0x1F, 0xFF,
        
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF
    };
    
               
    static HBITMAP  hBitmap;
    static int      cxClient, cyClient, cxSource, cySource;
    HDC hdc,        hdcMem;
    int             x, y;
    PAINTSTRUCT     ps;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            bitmap.bmBits = bits;
            
            hBitmap = CreateBitmapIndirect(&bitmap);
            
            cxSource = bitmap.bmWidth;
            cySource = bitmap.bmHeight;
            
            return(0);
            
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return(0);
            
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            
            hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hBitmap);
            
            for(y = 0; y < cyClient; y = y + cySource)
            {
                for(x = 0; x < cxClient; x = x + cxSource)
                {
                    BitBlt(hdc, x, y, cxSource, cySource, hdcMem, 0, 0, SRCCOPY);
                }
            }
            
            DeleteDC(hdcMem);
            EndPaint(hwnd, &ps);
            return(0);
            
        case WM_DESTROY:
            DeleteObject(hBitmap);
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}

