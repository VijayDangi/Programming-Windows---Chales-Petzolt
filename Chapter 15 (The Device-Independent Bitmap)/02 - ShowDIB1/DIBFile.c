/*-------------------------------------
    DIBFile.c :- DIB File Functions
-------------------------------------*/

//Headers
#include <windows.h>
#include <commdlg.h>
#include "dibfile.h"

//library
#pragma comment(lib, "ComDlg32.Lib")

//global variable declarations
static OPENFILENAME ofn;

//
//DibFileInitialize()
//
void DibFileInitialize(HWND hwnd)
{
    //variable declarations
    static TCHAR szFilter[] = TEXT("Bitmap Files (*.BMP)\0*.bmp\0")
                              TEXT("All Files (*.*)\0*.*\0\0");

    //code
    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = hwnd;
    ofn.hInstance         = NULL;
    ofn.lpstrFilter       = szFilter;
    ofn.lpstrCustomFilter = NULL;
    ofn.nMaxCustFilter    = 0;
    ofn.nFilterIndex      = 0;
    ofn.lpstrFile         = NULL;
    ofn.nMaxFile          = MAX_PATH;
    ofn.lpstrFileTitle    = NULL;
    ofn.nMaxFileTitle     = MAX_PATH;
    ofn.lpstrInitialDir   = NULL;
    ofn.lpstrTitle        = NULL;
    ofn.Flags             = 0;
    ofn.nFileExtension    = 0;
    ofn.lpstrDefExt       = TEXT("bmp");
    ofn.lCustData         = 0;
    ofn.lpfnHook          = NULL;
    ofn.lpTemplateName    = NULL;
}

//
//DibFileOpenDlg()
//
BOOL DibFileOpenDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
    //code
    ofn.hwndOwner      = hwnd;
    ofn.lpstrFile      = pstrFileName;
    ofn.lpstrFileTitle = pstrTitleName;
    ofn.Flags          = 0;
    
    return(GetOpenFileName(&ofn));
}

//
//DibFileSaveDlg()
//
BOOL DibFileSaveDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
    //code
    ofn.hwndOwner      = hwnd;
    ofn.lpstrFile      = pstrFileName;
    ofn.lpstrFileTitle = pstrTitleName;
    ofn.Flags          = OFN_OVERWRITEPROMPT;
    
    return(GetSaveFileName(&ofn));
}

//
//DibLoadImage()
//
BITMAPFILEHEADER *DibLoadImage(PTSTR pstrFileName)
{
    //variable declarations
    BOOL              bSuccess;
    DWORD             dwFileSize, dwHighSize, dwBytesRead;
    HANDLE            hFile;
    BITMAPFILEHEADER *pbmfh;
    
    //code
    hFile = CreateFile(pstrFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    
    if(hFile == INVALID_HANDLE_VALUE)
        return(NULL);
    
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
    
    if(!bSuccess || (dwBytesRead != dwFileSize)
                 || (pbmfh->bfType != *(WORD*)"BM")
                 || (pbmfh->bfSize != dwFileSize)
    )
    {
        free(pbmfh);
        return(NULL);
    }
    return(pbmfh);
}

//
//DibSaveImage()
//
BOOL DibSaveImage(PTSTR pstrFileName, BITMAPFILEHEADER *pbmfh)
{
    //variable declarations
    BOOL   bSuccess;
    DWORD  dwBytesWritten;
    HANDLE hFile;
    
    //code
    hFile = CreateFile(pstrFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if(hFile == INVALID_HANDLE_VALUE)
        return(FALSE);
    
    bSuccess = WriteFile(hFile, pbmfh, pbmfh->bfSize, &dwBytesWritten, NULL);
    CloseHandle(hFile);
    
    if(!bSuccess || (dwBytesWritten != pbmfh->bfSize))
    {
        DeleteFile(pstrFileName);
        return(FALSE);
    }
    return(TRUE);
}
