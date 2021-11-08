/*------------------------------------------------
    FormFeed.c :- Advances printer to next page
------------------------------------------------*/

//headers
#include <windows.h>

//global function declarations
HDC GetPrinterDC(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR iCmdLine, int iCmdShow)
{
    //variable declarations
    static DOCINFO di = { sizeof(DOCINFO), TEXT("FormFeed") };
    HDC            hdcPrint = GetPrinterDC();
    
    //code
    if(hdcPrint != NULL)
    {
        if(StartDoc(hdcPrint, &di) > 0)
            if(StartPage(hdcPrint) > 0 && EndPage(hdcPrint) > 0)
                EndDoc(hdcPrint);
            
        DeleteDC(hdcPrint);
    }
    return(0);
}
