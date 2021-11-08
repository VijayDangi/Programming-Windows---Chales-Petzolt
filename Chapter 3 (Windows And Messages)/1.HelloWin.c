#include <windows.h>
//#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("HelloWin");
	HWND         hWnd;
	MSG          msg;
	WNDCLASSEX   wndclass;
	
	wndclass.cbSize        = sizeof(WNDCLASSEX);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName  = NULL;
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
 	wndclass.hInstance     = hInstance;
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Thid program requires Windows NT and above"), szAppName, MB_ICONERROR);
		return (0);
	}
	
	hWnd = CreateWindow(szAppName,                  //window class name
											TEXT("The Hello Program"),  //window caption
											WS_OVERLAPPEDWINDOW,        //window style
											0,              //initial x position
											0,              //initial y position
											CW_USEDEFAULT,              //initial x size / width
											CW_USEDEFAULT,              //initial y size / height
											NULL,                       //parent window handle
											NULL,                       //window menu handle
											hInstance,                  //program instance handle
											NULL);							        //creation parameters
				
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd); 
	//FILE *fp = fopen("message.txt", "w");
	while(GetMessage(&msg, NULL, 0, 0))
	{
		//fprintf(fp, "time: %d \t x: %d \t y: %d\n", msg.time, msg.pt.x, msg.pt.y);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
		//fclose(fp);
	return (msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;
	
	switch(message)
	{
		case WM_CREATE:
			//PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
			return (0);
			
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			GetClientRect(hWnd, &rect);
			
			DrawText(hdc,
			         TEXT("Hello, Windows 10!"),
							 -1,
							 &rect,
							 DT_SINGLELINE | DT_CENTER | DT_VCENTER
							);
							
			EndPaint(hWnd, &ps);
			
			return (0);
			
		case WM_DESTROY:
			PostQuitMessage(0);
				return(0);
	}
	
	return(DefWindowProc(hWnd, message, wParam, lParam));
}
