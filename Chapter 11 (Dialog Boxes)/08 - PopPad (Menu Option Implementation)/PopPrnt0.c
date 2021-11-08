/*----------------------------------------------------------
    PopPrnt0.c :- Popup Printing Function (dummy version)
----------------------------------------------------------*/

//Header
#include <windows.h>
#include <stdio.h>

//global variable declaraions
extern FILE *gpFile;

//PopPrntPrintFile()
BOOL PopPrntPrintFile(HINSTANCE hInstance, HWND hwnd, HWND hwndEdit, PTSTR pstrTitleName)
{
    fprintf(gpFile, "PopPrnt0.c PopPrntPrintFile()\n");
    return(FALSE);
}
