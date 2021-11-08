/*--------------------------------------------------
    Colors3.c :- Version using Common Dialog Box
--------------------------------------------------*/

//Header
#include <windows.h>
#include <commdlg.h>

//macro
#pragma comment(lib, "comdlg32.lib")

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdLine)
{
    static CHOOSECOLOR cc;
    static COLORREF    crCustColors[16];
    
    cc.lStructSize    = sizeof(CHOOSECOLOR);
    cc.hwndOwner      = NULL;
    cc.hInstance      = NULL;
    cc.rgbResult      = RGB(0x80, 0x80, 0x80);
    cc.lpCustColors   = crCustColors;
    cc.Flags          = CC_RGBINIT | CC_FULLOPEN;
    cc.lCustData      = 0;
    cc.lpfnHook       = NULL;
    cc.lpTemplateName = NULL;
    
    return(ChooseColor(&cc));
}
