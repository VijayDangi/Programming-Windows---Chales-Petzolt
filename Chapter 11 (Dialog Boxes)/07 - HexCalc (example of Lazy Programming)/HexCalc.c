/*----------------------------------------
    HEXCALC.c - Hexadecimal Calculator
----------------------------------------*/

//Headers
#include <windows.h>

//global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
    //variable declarations
    TCHAR    szAppName[] = TEXT("HexCalc");
    HWND     hwnd;
    MSG      msg;
    WNDCLASS wndclass;
    
    //code
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = DLGWINDOWEXTRA; /* NOTE!!!! */
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, szAppName);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    
    if(!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_OK | MB_ICONERROR);
        exit(0);
    }
    
    hwnd = CreateDialog(hInstance, szAppName, 0, NULL);
    
    ShowWindow(hwnd, iCmdShow);
    
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return((int)msg.wParam);
}

//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //function declarations
    void  ShowNumber(HWND, UINT);
    DWORD CalcIt(UINT, int, UINT);
    
    //variable declarations
    static BOOL bNewNumber = TRUE;
    static int  iOperation = '=';
    static UINT iNumber, iFirstNum;
    HWND        hButton;
    
    //code
    switch(message)
    {
        case WM_KEYDOWN:    //left arrow --> backspace
            if(wParam != VK_LEFT)
                break;
            wParam = VK_BACK;
            
                //fall through
        case WM_CHAR:
            if((wParam = (WPARAM) CharUpper((TCHAR*) wParam))  == VK_RETURN)
                wParam = '=';
        
            if(hButton = GetDlgItem(hwnd, wParam))
            {
                SendMessage(hButton, BM_SETSTATE, 1, 0);
                Sleep(100);
                SendMessage(hButton, BM_SETSTATE, 0, 0);
            }
            else
            {
                MessageBeep(0);
                break;
            }
        
            //Fall through
        case WM_COMMAND:
            SetFocus(hwnd);
            
            if(LOWORD(wParam) == VK_BACK)   //backspace
                ShowNumber(hwnd, iNumber /= 16);
                
            else if(LOWORD(wParam) == VK_ESCAPE)    //escape
                ShowNumber(hwnd, iNumber = 0);
                
            else if(isxdigit(LOWORD(wParam)))   //hex digit
            {
                if(bNewNumber)
                {
                    iFirstNum = iNumber;
                    iNumber = 0;
                }
                bNewNumber = FALSE;
                
                if(iNumber <= MAXDWORD >> 4)
                {
                    ShowNumber(
                            hwnd,
                            iNumber = 16 * iNumber + wParam - (isdigit(wParam) ? '0' : 'A' - 10)
                        );
                }
                else
                    MessageBeep(0);
            }
            else
            {
                if(!bNewNumber)
                    ShowNumber(hwnd, iNumber = CalcIt(iFirstNum, iOperation, iNumber));
                bNewNumber = TRUE;
                iOperation = LOWORD(wParam);
            }
            
            return(0);
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }
    
    return(DefWindowProc(hwnd, message, wParam, lParam));
}


//ShowNumber()
void ShowNumber(HWND hwnd, UINT iNumber)
{
    //variable declarations
    TCHAR szBuffer[20];
    
    //code
    wsprintf(szBuffer, TEXT("%X"), iNumber);
    SetDlgItemText(hwnd, VK_ESCAPE, szBuffer); //VK_ESCAPE = 27
}

//CalcIt()
DWORD CalcIt(UINT iFirstNum, int iOperation, UINT iNum)
{
    //code
    switch(iOperation)
    {
        case '=':   return(iNum);
        case '+':   return(iFirstNum + iNum);
        case '-':   return(iFirstNum - iNum);
        case '*':   return(iFirstNum * iNum);
        case '&':   return(iFirstNum & iNum);
        case '|':   return(iFirstNum | iNum);
        case '^':   return(iFirstNum ^ iNum);
        case '<':   return(iFirstNum << iNum);
        case '>':   return(iFirstNum >> iNum);
        case '/':   return(iNum ? iFirstNum / iNum : MAXDWORD);
        case '%':   return(iNum ? iFirstNum % iNum : MAXDWORD);
        default:    return(0);
    }
}
