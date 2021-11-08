/*---------------------------------------------
    PopFont.c :- Popup Editor Font Functions
---------------------------------------------*/

//Headers
#include <windows.h>
#include <commdlg.h>
#include <stdio.h>

//macro
static LOGFONT logFont;
static HFONT   hFont;

extern FILE *gpFile;

//PopFontChooseFont()
BOOL PopFontChooseFont(HWND hwnd)
{
    //variable declarations
    CHOOSEFONT cf;
    
    //code
    fprintf(gpFile, "PopFont.c PopFontChooseFont(): Enter\n");
    
    cf.lStructSize    = sizeof(CHOOSEFONT);
    cf.hwndOwner      = hwnd;
    cf.hDC            = NULL;
    cf.lpLogFont      = &logFont;
    cf.iPointSize     = 0;
    cf.Flags          = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
    cf.rgbColors      = 0;
    cf.lCustData      = 0;
    cf.lpfnHook       = NULL;
    cf.lpTemplateName = NULL;
    cf.hInstance      = NULL;
    cf.lpszStyle      = NULL;
    cf.nFontType      = 0;       //Returned from ChooseFont()
    cf.nSizeMin       = 0;
    cf.nSizeMax       = 0;
    
    fprintf(gpFile, "PopFont.c PopFontChooseFont(): Return\n");
    
    return(ChooseFont(&cf));
}

//PopFontInitialize()
void PopFontInitialize(HWND hwndEdit)
{
    //code
    fprintf(gpFile, "PopFont.c PopFontInitialize(): Enter\n");
    
    GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), (PTSTR) &logFont);
    
    hFont = CreateFontIndirect(&logFont);
    SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, 0);
    
    fprintf(gpFile, "PopFont.c PopFontInitialize(): Return\n");
}

//PopFontSetFont()
void PopFontSetFont(HWND hwndEdit)
{
    //variable declarations
    HFONT hFontNew;
    RECT  rect;
    
    //code
    fprintf(gpFile, "PopFont.c PopFontSetFont(): Enter\n");
    
    hFontNew = CreateFontIndirect(&logFont);
    SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFontNew, 0);
    DeleteObject(hFont);
    
    hFont = hFontNew;
    GetClientRect(hwndEdit, &rect);
    InvalidateRect(hwndEdit, &rect, TRUE);
    
    fprintf(gpFile, "PopFont.c PopFontSetFont(): Return\n");
}

//PopFontDeinitialize()
void PopFontDeinitialize(void)
{
    fprintf(gpFile, "PopFont.c PopFontDeinitialize()\n");
    DeleteObject(hFont);
}

