/*
 * Clover Drawing Program Using Regions
 */

#include <windows.h>
#include <math.h>
#include <stdio.h>

#define PI 3.141596

#define XDIRECTION 10
#define YDIRECTION 20

float angle = 0.0f;;
int direction = YDIRECTION;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //function declaration
    void TransformAndDraw(HWND);
    
    //variable declaration
	TCHAR      szAppName[] = TEXT("TRANSFORMATION");
	HWND       hWnd;
	MSG        msg;
	WNDCLASSEX wndclass;
	
    //code
	wndclass.cbSize        = sizeof(WNDCLASSEX);
	wndclass.style         = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName  = NULL;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hInstance     = hInstance;
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	
	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Error while registering class."), szAppName, MB_ICONERROR);
		return(0);
	}
	
	hWnd = CreateWindow(szAppName,
						szAppName,
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
					
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	
	while(TRUE)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            TransformAndDraw(hWnd);
            angle += 1.0f;
        }
	}
	
	return(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //function declaration
    void TransformAndDraw(HWND);
    
    //variable declaration
	static  int  cxClient, cyClient;
 	HDC          hdc;
	PAINTSTRUCT  ps;
	
	switch(message)
	{
        
		case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		
			return(0);
		}
		
		case WM_PAINT:
		{
            
			hdc = BeginPaint(hWnd, &ps);
    
                TransformAndDraw(hWnd);
                angle += 0.1f;
            
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


/************ TransformAndDraw() *************/
void TransformAndDraw(HWND hWnd)
{
    HDC   hDC;
    XFORM xForm;
    RECT  rect;
    
    // TCHAR str[128];
    
    //Retrieve a DC handle for the application's window.
    hDC = GetDC(hWnd);
    
        //Set Mapping mode to MM_LOENGLISH. This moves the 
        //client area origin from the upper left corner of the window
        //to the lower left corner(this also reorients the y-axis so the drawing
        // operations occur in a true Cartesian space).
        //It guarantees portability so that the object draw retains its  dimensions 
        // on any display.
        
        SetGraphicsMode(hDC, GM_ADVANCED);
        SetMapMode(hDC, MM_LOENGLISH);
        
        if(direction == XDIRECTION)
        {
            xForm.eM11 = (FLOAT)  cos(PI * angle / 180);
            xForm.eM12 = (FLOAT) -sin(PI * angle / 180);
            xForm.eM21 = (FLOAT)  sin(PI * angle / 180);
            xForm.eM22 = (FLOAT)  cos(PI * angle / 180);
            xForm.eDx  = (FLOAT)  0.0;
            xForm.eDy  = (FLOAT)  0.0;
            
        }
        else if(direction == YDIRECTION)
        {
            xForm.eM11 = (FLOAT)  cos(PI * angle / 180);
            xForm.eM12 = (FLOAT)  sin(PI * angle / 180);
            xForm.eM21 = (FLOAT) -sin(PI * angle / 180);
            xForm.eM22 = (FLOAT)  cos(PI * angle / 180);
            xForm.eDx  = (FLOAT)  0.0;
            xForm.eDy  = (FLOAT)  0.0;
        }

        SetWorldTransform(hDC, &xForm);
        
        //fond the midpoint of the client area.
        GetClientRect(hWnd, (LPRECT)&rect);
        DPtoLP(hDC, (LPPOINT)&rect, 2);
        
        
        RECT rc = {rect.right/2 - 150, rect.bottom/2 - 150, rect.right/2 + 150, rect.bottom/2 + 150 };
        FillRect(hDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
        //Set a hollow brush.
        SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));
        if(angle/20 == 0)
            SelectObject(hDC, GetStockObject(WHITE_PEN));
        
        //Draw the exterior circle.
        Ellipse(hDC, (rect.right/2 - 100), (rect.bottom/2 + 100),
                     (rect.right/2 + 100), (rect.bottom/2 - 100));
                     
        //Draw the interior circle.
        Ellipse(hDC, (rect.right/2 - 94), (rect.bottom/2 + 94),
                     (rect.right/2 + 94), (rect.bottom/2 - 94));
                     
        //Draw the key.
        Rectangle(hDC, (rect.right/2 - 13), (rect.bottom/2 + 113),
                       (rect.right/2 + 13), (rect.bottom/2 + 50));
                       
        Rectangle(hDC, (rect.right/2 - 13), (rect.bottom/2 + 96),
                       (rect.right/2 + 13), (rect.bottom/2 + 50));
        
        //Draw the horizontal lines.
        MoveToEx(hDC, (rect.right/2 - 150), (rect.bottom/2 +   0), NULL);
        LineTo  (hDC, (rect.right/2 -  16), (rect.bottom/2 +   0));
        
        MoveToEx(hDC, (rect.right/2 -  13), (rect.bottom/2 +   0), NULL);
        LineTo  (hDC, (rect.right/2 +  13), (rect.bottom/2 +   0));
        
        MoveToEx(hDC, (rect.right/2 +  16), (rect.bottom/2 +   0), NULL);
        LineTo  (hDC, (rect.right/2 + 150), (rect.bottom/2 +   0));
        
        //Draw the vertical lines
        MoveToEx(hDC, (rect.right/2 +   0), (rect.bottom/2 - 150), NULL);
        LineTo  (hDC, (rect.right/2 +   0), (rect.bottom/2 -  16));
        
        MoveToEx(hDC, (rect.right/2 +   0), (rect.bottom/2 -  13), NULL);
        LineTo  (hDC, (rect.right/2 +   0), (rect.bottom/2 +  13));
        
        MoveToEx(hDC, (rect.right/2 +   0), (rect.bottom/2 +  16), NULL);
        LineTo  (hDC, (rect.right/2 +   0), (rect.bottom/2 + 150));
    
    ReleaseDC(hWnd, hDC);
}
