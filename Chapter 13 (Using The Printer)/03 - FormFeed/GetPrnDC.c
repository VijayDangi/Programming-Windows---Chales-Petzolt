/*----------------------------------------
    GetPrnDC.c :- GetPrinterDC function
----------------------------------------*/

//Header
#include <windows.h>

#pragma comment(lib, "Winspool.lib")

HDC GetPrinterDC(void)
{
    //variable declarations
    DWORD           dwNeeded, dwReturned;
    HDC             hdc;
    PRINTER_INFO_4 *pInfo4;
    PRINTER_INFO_5 *pInfo5;
    
    //code
    if(GetVersion()  & 0x80000000)  //Windows 98
    {
        EnumPrinters(PRINTER_ENUM_DEFAULT, NULL, 5, NULL, 0, &dwNeeded, &dwReturned);
        
        pInfo5 = malloc(dwNeeded);
        
        EnumPrinters(PRINTER_ENUM_DEFAULT, NULL, 5, (PBYTE) pInfo5, dwNeeded, &dwNeeded, &dwReturned);
        
        hdc = CreateDC(NULL, pInfo5->pPrinterName, NULL, NULL);
        
        free(pInfo5);
    }
    else        //Windows NT
    {
        EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 4, NULL, 0, &dwNeeded, &dwReturned);
        
        pInfo4 = malloc(dwNeeded);
        
        EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 4, (PBYTE) pInfo4, dwNeeded, &dwNeeded, &dwReturned);
        
        hdc = CreateDC(NULL, pInfo4->pPrinterName, NULL, NULL);
        
        free(pInfo4);
    }
    
    return(hdc);
}
