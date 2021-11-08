/*
    Arc, Chord, Pie
 */

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    TCHAR      szAppName[] = TEXT("Arc");
    HWND       hWnd;
    MSG        msg;
    WNDCLASSEX wndclass;
    
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszClassName = szAppName;
    wndclass.lpszMenuName = NULL;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    
    if(!RegisterClassEx(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK);
        return(-1);
    }
    
    // wndclass.lpszClassName = TEXT("CHILD");
    // wndclass.style = CS_HREDRAW | CS_VREDRAW;
    // if(!RegisterClassEx(&wndclass))
    // {
        // MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK);
        // return(-1);
    // }
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("Arch, Chord, Pie"),
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
            
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC         hDC;
    PAINTSTRUCT ps;
    // static int i = 0;
    switch(message)
    {
        // case WM_CREATE:
        
            // if(i == 0)
            // {
                // i++;
                // if(! CreateWindow(
                        // TEXT("CHILD"),
                        // TEXT("1Arch, Chord, Pie"),
                        // WS_VISIBLE | WS_CHILDWINDOW | WS_CAPTION  | WS_OVERLAPPEDWINDOW,
                        // 10,
                        // 10,
                        // 200,
                        // 200,
                        // hWnd,
                        // (HMENU)10,
                        // ((LPCREATESTRUCT)lParam)->hInstance,
                        // NULL
                    // ))
                    // {
                        // MessageBox(hWnd, TEXT("Error"), TEXT("EE"), MB_OK);
                    // }
            // }
                // return(0);
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
                SelectObject(hDC, GetStockObject(DC_PEN));
                SetDCPenColor(hDC, RGB(255, 128, 129));
                Arc  (hDC,   5,   5, 205, 305,  50, 50, 100, 100);
                
                SelectObject(hDC, GetStockObject(GRAY_BRUSH));
                Chord(hDC, 215, 315, 405, 605, 250, 350, 350, 500);
                
                SelectObject(hDC, GetStockObject(DKGRAY_BRUSH));
                Pie  (hDC, 415, 215, 605, 505, 450, 350, 550, 500);
            EndPaint(hWnd, &ps);
            return(0);
        
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}
