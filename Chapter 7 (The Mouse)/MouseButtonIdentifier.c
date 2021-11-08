#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
int main()
{
	TCHAR      szAppName[] = TEXT("MouseButtonIdentifier");
	HWND       hWnd;
	MSG        msg;
	WNDCLASSEX wndclass;
	HINSTANCE  hInstance = GetModuleHandle(NULL);
	
	wndclass.cbSize        = sizeof(WNDCLASSEX);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName  = NULL;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hIcon         = LoadIcon(NULL, IDC_ARROW);
	wndclass.hIconSm       = LoadIcon(NULL, IDC_ARROW);
	wndclass.hInstance     = hInstance;
	wndclass.hCursor       = LoadCursor(NULL, IDI_APPLICATION);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registerig class."), szAppName, MB_OK);
		return(0);
	}
	
	hWnd = CreateWindow(
				szAppName,
				TEXT("MOUSE MESSAGES"),
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				NULL,
				NULL,
				hInstance,
				NULL
			);
			
	if(!hWnd)
	{
		MessageBox(NULL, TEXT("Error while creating window."), szAppName, MB_OK);
		return(0);
	}
	
	// ShowWindow(hWnd, iCmdShow);
	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	
	static TCHAR *szMouseMessage[] = {
		TEXT("WM_MOUSEMOVE"),
		TEXT("WM_LBUTTONDOWN"),
		TEXT("WM_LBUTTONUP"),
		TEXT("WM_LBUTTONDBLCLK"),
		TEXT("WM_RBUTTONDOWN"),
		TEXT("WM_RBUTTONUP"),
		TEXT("WM_RBUTTONDBLCLK"),
		TEXT("WM_MBUTTONDOWN"),
		TEXT("WM_MBUTTONUP"),
		TEXT("WM_MBUTTONDBLCLK"),
		TEXT("WM_MOUSEWHEEL"),
		TEXT("WM_XBUTTONDOWN"),
		TEXT("WM_XBUTTONUP"),
		TEXT("WM_XBUTTONDBLCLK"),
	};
	
	static TCHAR *szNCMouseMessage[] = {
		TEXT("WM_NCMOUSEMOVE"),
		TEXT("WM_NCLBUTTONDOWN"),
		TEXT("WM_NCLBUTTONUP"),
		TEXT("WM_NCLBUTTONDBLCLK"),
		TEXT("WM_NCRBUTTONDOWN"),
		TEXT("WM_NCRBUTTONUP"),
		TEXT("WM_NCRBUTTONDBLCLK"),
		TEXT("WM_NCMBUTTONDOWN"),
		TEXT("WM_NCMBUTTONUP"),
		TEXT("WM_NCMBUTTONDBLCLK")
	};
	
	// #define WM_NCMOUSEMOVE                  0x00A0
	// #define WM_NCLBUTTONDOWN                0x00A1
	// #define WM_NCLBUTTONUP                  0x00A2
	// #define WM_NCLBUTTONDBLCLK              0x00A3
	// #define WM_NCRBUTTONDOWN                0x00A4
	// #define WM_NCRBUTTONUP                  0x00A5
	// #define WM_NCRBUTTONDBLCLK              0x00A6
	// #define WM_NCMBUTTONDOWN                0x00A7
	// #define WM_NCMBUTTONUP                  0x00A8
	// #define WM_NCMBUTTONDBLCLK              0x00A9
	
	TCHAR str[18];
	
	switch(message)
	{
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_MOUSEWHEEL:
			// wsprintf(str, TEXT("%s"), szMouseMessage[message - WM_MOUSEFIRST]);
			//MessageBox(hWnd, str, str, MB_OK);
			printf("Message: %s, x[LOWORD(lParam)] = %d, y[HIWORD(lParam)] = %d\n", szMouseMessage[message - WM_MOUSEFIRST], LOWORD(lParam), HIWORD(lParam));
			break;
		
		case WM_NCMOUSEMOVE:
		case WM_NCLBUTTONDOWN:
		case WM_NCLBUTTONUP:
		case WM_NCLBUTTONDBLCLK:
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:
		case WM_NCRBUTTONDBLCLK:
		case WM_NCMBUTTONDOWN:
		case WM_NCMBUTTONUP:
		case WM_NCMBUTTONDBLCLK:
			printf("Message: %s, HitTest: ", szNCMouseMessage[message - WM_NCMOUSEMOVE]);
			
			switch(wParam)
			{
				case HTERROR:
					printf("HTERROR, ");
					break;
				case HTTRANSPARENT:
					printf("HTTRANSPARENT, ");
					break;
				case HTNOWHERE:
					printf("HTNOWHERE, ");
					break;
				case HTCLIENT:
					printf("HTCLIENT, ");
					break;
				case HTCAPTION:
					printf("HTCAPTION, ");
					break;
				case HTSYSMENU:
					printf("HTSYSMENU, ");
					break;
				case HTGROWBOX:
					printf("HTGROWBOX HTSIZE, ");
					break;
				case HTMENU:
					printf("HTMENU, ");
					break;
				case HTHSCROLL:
					printf("HTHSCROLL, ");
					break;
				case HTVSCROLL:
					printf("HTVSCROLL, ");
					break;
				case HTMINBUTTON:// HTREDUCE
					printf("HTMINBUTTON HTREDUCE, ");
					break;
				case HTMAXBUTTON: //HTZOOM
					printf("HTMAXBUTTON HTZOOM, ");
					break;
				case HTLEFT: //HTSIZEFIRST
					printf("HTLEFT HTSIZEFIRST, ");
					break;
				case HTRIGHT:
					printf("HTRIGHT, ");
					break;
				case HTTOP:
					printf("HTTOP, ");
					break;
				case HTTOPLEFT:
					printf("HTTOPLEFT, ");
					break;
				case HTTOPRIGHT:
					printf("HTTOPRIGHT, ");
					break;
				case HTBOTTOM:
					printf("HTBOTTOM, ");
					break;
				case HTBOTTOMLEFT:
					printf("HTBOTTOMLEFT, ");
					break;
				case HTBOTTOMRIGHT: //HTSIZELAST
					printf("HTBOTTOMRIGHT HTSIZELAST, ");
					break;
				case HTBORDER:
					printf("HTBORDER, ");
					break;
				case HTOBJECT:
					printf("HTOBJECT, ");
					break;
				case HTCLOSE:
					printf("HTCLOSE, ");
					break;
				case HTHELP:
					printf("HTHELP, ");
					break;
			}
			
			printf(" xScreen[LOWORD(lParam)] = %d, yScreen[HIWORD(lParam)] %d\n", LOWORD(lParam), HIWORD(lParam));
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ScreenToClient(hWnd, &pt);
			printf(" xClient[LOWORD(lParam)] = %d, yClient[HIWORD(lParam)] %d\n\n", pt.x, pt.y);
			
			break;
		
		case WM_NCHITTEST:
			printf("********* WM_NCHITTEST, xScreen: %d, yScreen: %d ***********\n", LOWORD(lParam), HIWORD(lParam));
			// return((LRESULT) HTNOWHERE); //process no mouse messages.
			break;
		
		case WM_CAPTURECHANGED:
			printf("WM_CAPTURECHANGED\n");
			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
