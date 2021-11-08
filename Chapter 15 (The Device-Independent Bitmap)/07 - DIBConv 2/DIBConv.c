/*------------------------------------------
    DIBConv.c :- Converts a DIB to a DDB
------------------------------------------*/

//headers
#include <windows.h>
#include <commdlg.h>
#include "Resource.h"

//library
#pragma comment(lib, "ComDlg32.lib")

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global vbariable declarations
TCHAR szAppName[] = TEXT("DibConv");

//
//WinMain()
//
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
    wndclass.lpszMenuName   = szAppName;
    wndclass.lpszClassName  = szAppName;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class"), szAppName, MB_ICONERROR);
        return(0);
    }
    
    hwnd = CreateWindow(
                szAppName,
                TEXT("DIB to DDB Conversion"),
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

//
//WndProc()
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //function declarations
    HBITMAP CreateBitmapObjectFromDibFile(HDC, PTSTR);
    
    //variable declarations
    static HBITMAP      hBitmap;
    static int          cxClient, cyClient;
    static OPENFILENAME ofn;
    static TCHAR        szFileName[MAX_PATH], szTitleName[MAX_PATH];
    static TCHAR        szFilter[] = TEXT("Bitmap File (*.BMP)\0*.bmp\0")
                                     TEXT("All Files (*.*)\0*.*\0\0");

    BITMAP      bitmap;
    HDC         hdc, hdcMem;
    PAINTSTRUCT ps;
    
    //code
    switch(message)
    {
        case WM_CREATE:
            ofn.lStructSize       = sizeof(OPENFILENAME);
            ofn.hwndOwner         = hwnd;
            ofn.hInstance         = NULL;
            ofn.lpstrFilter       = szFilter;
            ofn.lpstrCustomFilter = NULL;
            ofn.nMaxCustFilter    = 0;
            ofn.nFilterIndex      = 0;
            ofn.lpstrFile         = szFileName;
            ofn.nMaxFile          = MAX_PATH;
            ofn.lpstrFileTitle    = szTitleName;
            ofn.nMaxFileTitle     = MAX_PATH;
            ofn.lpstrInitialDir   = NULL;
            ofn.lpstrTitle        = NULL;
            ofn.Flags             = 0;
            ofn.nFileOffset       = 0;
            ofn.nFileExtension    = 0;
            ofn.lpstrDefExt       = TEXT("bmp");
            ofn.lCustData         = 0;
            ofn.lpfnHook          = NULL;
            ofn.lpTemplateName    = NULL;
            
            return(0);
            
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            return(0);
            
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_FILE_OPEN:
                    //Show the file open dialog box
                    if(!GetOpenFileName(&ofn))
                        return(0);
                    
                    //If there's an existing DIB, delte it
                    if(hBitmap)
                    {
                        DeleteObject(hBitmap);
                        hBitmap = NULL;
                    }
                    
                    //Create the DDB from the DIB
                    SetCursor(LoadCursor(NULL, IDC_WAIT));
                    ShowCursor(TRUE);
                    
                    hdc = GetDC(hwnd);
                        
                        hBitmap = CreateBitmapObjectFromDibFile(hdc, szFileName);
                        
                    ReleaseDC(hwnd, hdc);
                    
                    ShowCursor(FALSE);
                    SetCursor(LoadCursor(NULL, IDC_ARROW));
                    
                    //Invalidate the client are for later update
                    InvalidateRect(hwnd, NULL, TRUE);
                    
                    if(hBitmap == NULL)
                    {
                        MessageBox(hwnd, TEXT("Cannot load DIB file"), szAppName, MB_OK | MB_ICONEXCLAMATION);
                    }
                    
                    return(0);
            }
            
            break;
            
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            
                if(hBitmap)
                {
                    GetObject(hBitmap, sizeof(BITMAP), &bitmap);
                    
                    hdcMem = CreateCompatibleDC(hdc);
                    
                        SelectObject(hdcMem, hBitmap);
                        
                        BitBlt(    hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
                                hdcMem, 0, 0, SRCCOPY);

                    DeleteDC(hdcMem);
                }
            
            EndPaint(hwnd, &ps);
            
            return(0);
            
        case WM_DESTROY:
            if(hBitmap)
                DeleteObject(hBitmap);
            
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}


//
//CreateBitmapObjectFromDibFile()
//
HBITMAP CreateBitmapObjectFromDibFile(HDC hdc, PTSTR szFileName)
{
    //variable declarations
    BITMAPFILEHEADER *pbmfh;
    BITMAPINFOHEADER *pbmih;
    BOOL              bSuccess;
    DWORD             dwFileSize, dwHighSize, dwBytesRead;
    HANDLE            hFile;
    HBITMAP           hBitmap;
    HDC               hdcMem;
    int               cx, cy, iBitCount;
    
    //code
    
    //open the file: read access, prohibit write access
    hFile = CreateFile(
                szFileName,
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_FLAG_SEQUENTIAL_SCAN,
                NULL
            );

    if(hFile == INVALID_HANDLE_VALUE)
        return(NULL);
    
    //Read in the whole file
    dwFileSize = GetFileSize(hFile, &dwHighSize);
    
    if(dwHighSize)
    {
        CloseHandle(hFile);
        return(NULL);
    }
    
    pbmfh = malloc(dwFileSize);
    
    if(!pbmfh)
    {
        CloseHandle(hFile);
        return(NULL);
    }
    
    bSuccess = ReadFile(hFile, pbmfh, dwFileSize, &dwBytesRead, NULL);
    CloseHandle(hFile);
    
        //verify tyhe file
    if(!bSuccess || (dwBytesRead != dwFileSize)
                 || (pbmfh->bfType != *(WORD*) "BM")
                 || (pbmfh->bfSize != dwFileSize)
    )
    {
        free(pbmfh);
        pbmfh = NULL;
        return(NULL);
    }
    
    pbmih = (BITMAPINFOHEADER *) (pbmfh + 1);
    if(pbmih->biSize == sizeof(BITMAPCOREHEADER))
    {
        cx = ((PBITMAPCOREHEADER) pbmih)->bcWidth;
        cy = ((PBITMAPCOREHEADER) pbmih)->bcHeight;
        iBitCount = ((PBITMAPCOREHEADER) pbmih)->bcBitCount;
    }
    else
    {
        cx = pbmih->biWidth;
        cy = pbmih->biHeight;
        iBitCount = pbmih->biBitCount;
    }
    
    if(hdc)
        hBitmap = CreateCompatibleBitmap(hdc, cx, cy);
    else
        hBitmap = CreateBitmap(cx, cy, 1, 1, NULL);

    
    hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hBitmap);
    SetDIBitsToDevice(
            hdcMem,
            0,
            0,
            cx,
            cy,
            0,
            0,
            0,
            cy,
            (BYTE*) pbmfh + pbmfh->bfOffBits,
            (BITMAPINFO*) pbmih,
            DIB_RGB_COLORS
        );
    DeleteDC(hdcMem);
                
    free(pbmfh);
    return(hBitmap);

}
