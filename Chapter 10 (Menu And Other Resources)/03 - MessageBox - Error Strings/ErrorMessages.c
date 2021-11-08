//Header
#include <windows.h>
#include "resource.h"

TCHAR szAppName[] = TEXT("Error Message Demo");

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //function declarations
    int OkMessage(HWND, int, TCHAR*);
    
    OkMessage(NULL, IDS_FILENOTFOUND, TEXT("abc.txt"));
    OkMessage(NULL, IDS_FILETOOBIG, TEXT("pqr.txt"));
    OkMessage(NULL, IDS_FILEREADONLY, TEXT("xyz.txt"));\
    
    return(0);
}

//OkMessage
int OkMessage(HWND hwnd, int iErrorNumber, TCHAR *szFileName)
{
    TCHAR szFormat[40];
    TCHAR szBuffer[60];
    
    LoadString(GetModuleHandle(NULL), iErrorNumber, szFormat, 40);
    wsprintf(szBuffer, szFormat, szFileName);
    
    return(MessageBox(hwnd, szBuffer, szAppName, MB_OK | MB_ICONEXCLAMATION));
}
