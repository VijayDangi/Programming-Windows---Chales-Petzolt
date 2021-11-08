/*--------------------------------------------
    Print2.c - Printing With Abort Procedure
--------------------------------------------*/

//Headers
#include <windows.h>

//global function declaration
HDC  GetPrinterDC(void);            //in GetPrnDC.c
void PageGDICalls(HDC, int, int);   //in Print.c

//global variable declaration
HINSTANCE hInst;
TCHAR     szAppName[] = TEXT("Print2");
TCHAR     szCaption[] = TEXT("Print Program 2 (About Procedure)");

//AbortProc()
BOOL CALLBACK AbortProc(HDC hdcPrn, int iCode)
{
    //variable declaration
    MSG msg;
    
    //code
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return(TRUE);
}

//PrintMyPage()
BOOL PrintMyPage(HWND hwnd)
{
    //variable declaration
    static DOCINFO di = { sizeof(DOCINFO), TEXT("Print2: Printing") };
    BOOL           bSuccess = TRUE;
    HDC            hdcPrn;
    int            xPage, yPage;
    
    //code
    if(NULL == (hdcPrn = GetPrinterDC()))
        return(FALSE);
    
    xPage = GetDeviceCaps(hdcPrn, HORZRES);
    yPage = GetDeviceCaps(hdcPrn, VERTRES);
    
    EnableWindow(hwnd, FALSE); //Disable window
    
    SetAbortProc(hdcPrn, AbortProc); //set Abort Procedure
    
    if(StartDoc(hdcPrn, &di) > 0)
    {
        if(StartPage(hdcPrn) > 0)
        {
            PageGDICalls(hdcPrn, xPage, yPage);
            
            if(EndPage(hdcPrn) > 0)
                EndDoc(hdcPrn);
            else
                bSuccess = FALSE;
        }
    }
    else
        bSuccess = FALSE;
    
    EnableWindow(hwnd, TRUE);   //Enable Window
    
    DeleteDC(hdcPrn);
    return(bSuccess);
}
