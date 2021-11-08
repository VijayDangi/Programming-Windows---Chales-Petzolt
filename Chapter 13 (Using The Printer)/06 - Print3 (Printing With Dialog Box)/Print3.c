/*--------------------------------------------
    Print3.c - Printing With Dialog Box
--------------------------------------------*/

//Headers
#include <windows.h>

//global function declaration
HDC  GetPrinterDC(void);            //in GetPrnDC.c
void PageGDICalls(HDC, int, int);   //in Print.c

//global variable declaration
HINSTANCE hInst;
TCHAR     szAppName[] = TEXT("Print3");
TCHAR     szCaption[] = TEXT("Print Program 2 (Dialog Box)");

BOOL      bUserAbort;
HWND      hDlgPrint;

//PrintDlgProc()
BOOL CALLBACK PrintDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    //code
    switch(message)
    {
        case WM_INITDIALOG:
            SetWindowText(hDlg, szAppName);
            EnableMenuItem(GetSystemMenu(hDlg, FALSE), SC_CLOSE, MF_GRAYED);
            return(TRUE);
            
        case WM_COMMAND:
            bUserAbort = TRUE;
            EnableWindow(GetParent(hDlg), TRUE);
            DestroyWindow(hDlg);
            hDlgPrint = NULL;
            return(TRUE);
    }
    return(FALSE);
}

//AbortProc()
BOOL CALLBACK AbortProc(HDC hdcPrn, int iCode)
{
    //variable declaration
    MSG msg;
    
    //code
    while(!bUserAbort && PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if(!hDlgPrint || !IsDialogMessage(hDlgPrint, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    return(!bUserAbort);
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
    
    bUserAbort = FALSE;
    hDlgPrint  = CreateDialog(hInst, TEXT("PrintDlgBox"), hwnd, PrintDlgProc);
    
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
    
    if(!bUserAbort)
    {
        EnableWindow(hwnd, TRUE);   //Enable Window
        DestroyWindow(hDlgPrint);
    }
    
    DeleteDC(hdcPrn);
    
    return(bSuccess && !bUserAbort);
}
