/*--------------------------------------
    Print1.c - Bare Bones Printing
--------------------------------------*/

//Headers
#include <windows.h>

//global function declaration
HDC  GetPrinterDC(void);            //in GetPrnDC.c
void PageGDICalls(HDC, int, int);   //in Print.c

//global variable declaration
HINSTANCE hInst;
TCHAR     szAppName[] = TEXT("Print1");
TCHAR     szCaption[] = TEXT("Print Program 1");

//PrintMyPage()
BOOL PrintMyPage(HWND hwnd)
{
    //variable declaration
    static DOCINFO di = { sizeof(DOCINFO), TEXT("Print1: Printing") };
    BOOL           bSuccess = TRUE;
    HDC            hdcPrn;
    int            xPage, yPage;
    
    //code
    if(NULL == (hdcPrn = GetPrinterDC()))
        return(FALSE);
    
    xPage = GetDeviceCaps(hdcPrn, HORZRES);
    yPage = GetDeviceCaps(hdcPrn, VERTRES);
    
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
    
    DeleteDC(hdcPrn);
    return(bSuccess);
}
