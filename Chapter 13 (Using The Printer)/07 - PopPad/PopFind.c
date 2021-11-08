/*----------------------------------------------------------
    PopFind.c :- Popup Editor Search and Replace Functions
----------------------------------------------------------*/

//Headers
#include <windows.h>
#include <commdlg.h>
#include <tchar.h>      //for _tcsstr (strstr for Unicode and non-Unicode
#include <stdio.h>

//macro
#define MAX_STRING_LEN  256

//global variable declarations
static TCHAR szFindText[MAX_STRING_LEN];
static TCHAR szReplaceText[MAX_STRING_LEN];

extern FILE *gpFile;

//PopFindFindDlg()
HWND PopFindFindDlg(HWND hwnd)
{
    //variable declarations
    static FINDREPLACE fr;  //Must be static for modeless dialog!!!
    
    //code
    fprintf(gpFile, "PopFind.c PopFindFindDlg(): Enter\n");
    
    fr.lStructSize      = sizeof(FINDREPLACE);
    fr.hwndOwner        = hwnd;
    fr.hInstance        = NULL;
    fr.Flags            = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;
    fr.lpstrFindWhat    = szFindText;
    fr.lpstrReplaceWith = NULL;
    fr.wFindWhatLen     = MAX_STRING_LEN;
    fr.wReplaceWithLen  = 0;
    fr.lCustData        = 0;
    fr.lpfnHook         = NULL;
    fr.lpTemplateName   = NULL;
    
    fprintf(gpFile, "PopFind.c PopFindFindDlg(): Return\n");
    return(FindText(&fr));
}

//PopFindReplaceDlg()
HWND PopFindReplaceDlg(HWND hwnd)
{
    //variable declarations
    static FINDREPLACE fr;  //must be static for modeless dialog!!!
    
    //code
    fprintf(gpFile, "PopFind.c PopFindReplaceDlg(): Enter\n");
    
    fr.lStructSize      = sizeof(FINDREPLACE);
    fr.hwndOwner        = hwnd;
    fr.hInstance        = NULL;
    fr.Flags            = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;
    fr.lpstrFindWhat    = szFindText;
    fr.lpstrReplaceWith = szReplaceText;
    fr.wFindWhatLen     = MAX_STRING_LEN;
    fr.wReplaceWithLen  = MAX_STRING_LEN;
    fr.lCustData        = 0;
    fr.lpfnHook         = NULL;
    fr.lpTemplateName   = NULL;
    
    fprintf(gpFile, "PopFind.c PopFindReplaceDlg(): Return\n");
    return(ReplaceText(&fr));
}

//PopFindFindText()
int PopFindFindText(HWND hwndEdit, int *piSearchOffset, LPFINDREPLACE pfr)
{
    //variable declarations
    int   iLength, iPos;
    PTSTR pstrDoc, pstrPos;
    
    //code
    fprintf(gpFile, "PopFind.c PopFindFindText(): Enter\n");
    
        //Read in the edit document
    iLength = GetWindowTextLength(hwndEdit);
    
    if(NULL == (pstrDoc = (PTSTR)malloc((iLength + 1) * sizeof(TCHAR))))
    {
        fprintf(gpFile, "PopFind.c PopFindFindText(): Memory Allocation Error\n");
        return(FALSE);
    }
    
    GetWindowText(hwndEdit, pstrDoc, iLength + 1);
    
        //Search the document for the find string
    pstrPos = _tcsstr (pstrDoc + *piSearchOffset, pfr->lpstrFindWhat);
    free(pstrDoc);
    // pstrDoc = NULL;
    
        //Return an error code if the string cannot be found.
    if(pstrPos == NULL)
    {
        fprintf(gpFile, "PopFind.c PopFindFindText(): String Not Found\n");
        return(FALSE);
    }
    
        //Find the position in the document and the new start offset
    iPos = pstrPos - pstrDoc;
    *piSearchOffset = iPos + lstrlen(pfr->lpstrFindWhat);
    
        //Select the found text
    SendMessage(hwndEdit, EM_SETSEL, iPos, *piSearchOffset);
    SendMessage(hwndEdit, EM_SCROLLCARET, 0, 0);
    
    fprintf(gpFile, "PopFind.c PopFindFindText(): Return\n");
    return(TRUE);
}

//PopFindNextText()
BOOL PopFindNextText(HWND hwndEdit, int *piSearchOffset)
{
    //variable declarations
    FINDREPLACE fr;
    
    //code
    fprintf(gpFile, "PopFind.c PopFindNextText(): Enter\n");
    
    fr.lpstrFindWhat = szFindText;
    
    fprintf(gpFile, "PopFind.c PopFindNextText(): Return\n");
    return(PopFindFindText(hwndEdit, piSearchOffset, &fr));
}

//PopFindReplaceText()
BOOL PopFindReplaceText(HWND hwndEdit, int *piSearchOffset, LPFINDREPLACE pfr)
{
    //code
    fprintf(gpFile, "PopFind.c PopFindReplaceText(): Enter\n");
    
        //Find the text
    if(!PopFindFindText(hwndEdit, piSearchOffset, pfr))
        return(FALSE);
    
        //Replace it
    SendMessage(hwndEdit, EM_REPLACESEL, 0, (LPARAM) pfr->lpstrReplaceWith);
    
    fprintf(gpFile, "PopFind.c PopFindReplaceText(): Return\n");
    return(TRUE);
}

//PopFindValidFind()
BOOL PopFindValidFind(void)
{
    fprintf(gpFile, "PopFind.c PopFindValidFind()\n");
    return (*szFindText != '\0');
}
