/*-------------------------------------------
    DIBFILE.h :- header file for DIBFILE.c
-------------------------------------------*/

void DibFileInitialize(HWND hwnd);
BOOL DibFileOpenDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName);
BOOL DibFileSaveDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName);

BITMAPFILEHEADER *DibLoadImage(PTSTR pstrFileName);
BOOL DibSaveImage(PTSTR pstrFileName, BITMAPFILEHEADER*);
