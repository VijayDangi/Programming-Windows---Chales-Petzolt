/*
 * System Metrics Display
 */
#include <windows.h>
#include "sysmets.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR iCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("SysMets2");
	HWND         hWnd;
	MSG          msg;
	WNDCLASSEX   wndclass;
	
	wndclass.cbSize        = sizeof(wndclass);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName  = NULL;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registering class"), szAppName, MB_ICONERROR);
		return (0);
	}
	
	hWnd = CreateWindow(szAppName, //class name
						TEXT("System Metrics Display 2"), //Caption title
						WS_OVERLAPPEDWINDOW | WS_VSCROLL, // style
						CW_USEDEFAULT, //x - position
						CW_USEDEFAULT, //y - position
						CW_USEDEFAULT, // width
						CW_USEDEFAULT, // height
						NULL, //parent window handle
						NULL, //menu
						hInstance, //handle of instance of program
						NULL //lParam for creation
					);
	
	ShowWindow(hWnd, iCmdShow); //display window
	UpdateWindow(hWnd); //WM_PAINT -> paint window
	
	//Message Loop
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int  cxChar, cxCaps, cyChar, cyClient, iVscrollPos;
	HDC         hdc;
	int         i, y;
	PAINTSTRUCT ps;
	TCHAR       szBuffer[10];
	TEXTMETRIC  tm;
	
	switch(message)
	{
		case WM_CREATE:
		{
			hdc = GetDC(hWnd);
			
			GetTextMetrics(hdc, &tm);
			cxChar = tm.tmAveCharWidth;
			/*
				The low bit of the tmPitchAndFamily field in the TEXTMETRIC structure is 1 for a variable-width font,
				and 0 for a fixed-pitch font. this program uses this bit to calculate cxCaps from cxChar.
			*/
			cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
			cyChar = tm.tmHeight + tm.tmExternalLeading;
			
			ReleaseDC(hWnd, hdc);
			
			SetScrollRange(hWnd,         //handle of window
			               SB_VERT,      //type of scroll bar (vertical / horizontal)
			               0,            //minimum position
			               NUMLINES - 1, //maximum position
			               FALSE         //redraw
						);
						
			SetScrollPos(  hWnd,        //handle of window
			               SB_VERT,     //type of scroll bar (vertical / horizontal)
						   iVscrollPos, //position of scroll bar
						   TRUE         // redraw
						);
						
			return(0);
		}
		
		case WM_SIZE:
		{
			cyClient = HIWORD(lParam);
			return (0);
		}
		
		case WM_VSCROLL:
		{
			switch(LOWORD(wParam))
			{
				case SB_LINEUP:
					iVscrollPos -= 1;
					break;
				
				case SB_LINEDOWN:
					iVscrollPos += 1;
					break;
					
				case SB_PAGEUP:
					iVscrollPos -= cyClient / cyChar;
					break;
					
				case SB_PAGEDOWN:
					iVscrollPos += cyClient / cyChar;
					break;
					
				case SB_THUMBPOSITION:
					iVscrollPos = HIWORD(wParam);
					break;
			}
			
			iVscrollPos = max(0, min(iVscrollPos, NUMLINES - 1));
			
			if(iVscrollPos != GetScrollPos(hWnd, SB_VERT))
			{
				SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			
			return(0);
		}
		
		case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			
			for(i = iVscrollPos; i < NUMLINES; i++)
			{
				//y = cyChar * (i - iVscrollPos);
				
				TextOut(hdc,
						0,
						cyChar * i,
						sysmetrics[i].szLabel,
						lstrlen(sysmetrics[i].szLabel)
					);
					
				TextOut(hdc,
						22 * cxCaps,
						cyChar * i,
						sysmetrics[i].szDesc,
						lstrlen(sysmetrics[i].szDesc)
					);
					
				SetTextAlign(hdc, TA_RIGHT | TA_TOP);
				
				TextOut(hdc,
						22 * cxCaps + 40 * cxChar,
						cyChar * i,
						szBuffer,
						wsprintf(szBuffer, TEXT("%5d"), GetSystemMetrics(sysmetrics[i].iIndex))
					);
					
				SetTextAlign(hdc, TA_LEFT | TA_TOP);
			}
			
			EndPaint(hWnd, &ps);
			
			return(0);
		}
		
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return(0);
		}
	}
	
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
