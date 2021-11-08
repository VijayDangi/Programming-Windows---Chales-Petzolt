#include <windows.h>

#define DIVISIONS 5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    TCHAR      szAppName[] = TEXT("CHECKER");
    HWND       hWnd;
    MSG        msg;
    WNDCLASSEX wndclass;
    
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
        return(0);
    }
    
    hWnd = CreateWindow(
                szAppName,
                TEXT("CHECKER"),
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
            
    if(!hWnd)
    {
        MessageBox(NULL, TEXT("Error while creating window"), szAppName, MB_OK);
        return(0);
    }
    
    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return(msg.wParam);
}

void DrawCheckers(HDC hDC, int cxBlock, int cyBlock, BOOL fState[][5])
{
    int x, y;
    for(x = 0; x < DIVISIONS; x++)
    {
        for(y = 0; y < DIVISIONS; y++)
        {
            Rectangle(hDC, x      * cxBlock,  y      * cyBlock,
                          (x + 1) * cxBlock, (y + 1) * cyBlock);

            if(fState[x][y])
            {
                MoveToEx(hDC,  x      * cxBlock,  y      * cyBlock, NULL);
                LineTo  (hDC, (x + 1) * cxBlock, (y + 1) * cyBlock);
              
                MoveToEx(hDC, (x + 1) * cxBlock,  y      * cyBlock, NULL);
                LineTo  (hDC,  x      * cxBlock, (y + 1) * cyBlock);
            }
        }
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int  cxClient, cyClient;
    static int  cxBlock, cyBlock;
    static BOOL fState[DIVISIONS][DIVISIONS];
    static int  x, y;
    RECT        rc;
    HDC         hDC;
    PAINTSTRUCT ps;
    
    switch(message)
    {
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            
            
            
            cxBlock = cxClient / DIVISIONS;
            cyBlock = cyClient / DIVISIONS;
            
            hDC = GetDC(hWnd);
            
                DrawCheckers(hDC, cxBlock, cyBlock, fState);
                
            ReleaseDC(hWnd, hDC);
            break;
        
        case WM_LBUTTONDOWN:
            
            x = LOWORD(lParam) / cxBlock;
            y = HIWORD(lParam) / cyBlock;
            
            if(x < DIVISIONS && y < DIVISIONS)
            {
                fState[x][y] ^= 1;
                rc.left   =  x      * cxBlock;
                rc.top    =  y      * cyBlock;
                rc.right  = (x + 1) * cxBlock;
                rc.bottom = (y + 1) * cyBlock;
                
                InvalidateRect(hWnd, &rc, TRUE);
            }
            
            break;
        
        case WM_ERASEBKGND :
            DrawCheckers((HDC)wParam, cxBlock, cyBlock, fState);
            return(1L);
        
        case WM_PAINT:
            hDC = BeginPaint(hWnd, &ps);
            
                if(fState[x][y])
                {
                    MoveToEx(hDC,  x      * cxBlock,  y      * cyBlock, NULL);
                    LineTo  (hDC, (x + 1) * cxBlock, (y + 1) * cyBlock);
                  
                    MoveToEx(hDC, (x + 1) * cxBlock,  y      * cyBlock, NULL);
                    LineTo  (hDC,  x      * cxBlock, (y + 1) * cyBlock);
                }
         
            
            EndPaint(hWnd, &ps);
            break;
            return(1L);
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    
    return(DefWindowProc(hWnd, message, wParam, lParam));
}
