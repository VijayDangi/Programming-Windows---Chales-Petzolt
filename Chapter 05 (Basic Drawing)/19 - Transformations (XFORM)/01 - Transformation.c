/*
 * Clover Drawing Program Using Regions
 */

#include <windows.h>
#include <math.h>

#define TWO_PI (2.0 * 3.14159)

#define SCALE      0
#define TRANSLATE  1
#define ROTATE     2
#define SHEAR      3
#define REFLECT    4
#define NORMAL     5

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	TCHAR      szAppName[] = TEXT("TRANSFORMATION");
	HWND       hWnd;
	MSG        msg;
	WNDCLASSEX wndclass;
	
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
	
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //function declaration
    void TransformAndDraw(int, HWND);
    
    //variable declaration
	static  int  cxClient, cyClient;
    static  int  iTransform = NORMAL;
 	HDC          hdc;
	PAINTSTRUCT  ps;
	
	switch(message)
	{
        case WM_CHAR:
        {
            switch(wParam)
            {
                case 's':  //SCALE
                case 'S':
                    iTransform = SCALE;
                    break;
                    
                case 't':  //TRANSLATE
                case 'T':
                    iTransform = TRANSLATE;
                    break;
                    
                case 'r':  //Rotate
                case 'R':
                    iTransform = ROTATE;
                    break;
                    
                case 'h':  //SHEAR
                case 'H':
                    iTransform = SHEAR;
                    break;
                    
                case 'm':  //REFLECT
                case 'M':
                    iTransform = REFLECT;
                    break;
                    
                case 'n':
                case 'N':
                    iTransform = NORMAL;
                    break;
                    
                default:
                    break;
            }
            
            //TransformAndDraw(iTransform, hWnd);
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        }
		case WM_SIZE:
		{
			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		
			return(0);
		}
		
		case WM_PAINT:
		{
            
			hdc = BeginPaint(hWnd, &ps);
    
                TransformAndDraw(iTransform, hWnd);
            
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
void TransformAndDraw(int iTransform, HWND hWnd)
{
    HDC   hDC;
    XFORM xForm;
    RECT  rect;
    
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
        
        
        switch(iTransform)
        {
            case SCALE:     //scale to 1/2 of the original size.
                xForm.eM11 = (FLOAT) 0.5;
                xForm.eM12 = (FLOAT) 0.0;
                xForm.eM21 = (FLOAT) 0.0;
                xForm.eM22 = (FLOAT) 0.5;
                xForm.eDx  = (FLOAT) 0.0;
                xForm.eDx  = (FLOAT) 0.0;
                
                SetWorldTransform(hDC, &xForm);
                break;
                
            case TRANSLATE:  //Translate right by 3/4 inch
                xForm.eM11 = (FLOAT) 1.0;
                xForm.eM12 = (FLOAT) 0.0;
                xForm.eM21 = (FLOAT) 0.0;
                xForm.eM22 = (FLOAT) 1.0;
                xForm.eDx  = (FLOAT) 75.0;
                xForm.eDy  = (FLOAT) 0.0;
                
                SetWorldTransform(hDC, &xForm);
                break;
                
            case ROTATE:     //Rotate 30 degrees counter-clock wise.
                xForm.eM11 = (FLOAT)  0.8660;
                xForm.eM12 = (FLOAT)  0.5000;
                xForm.eM21 = (FLOAT) -0.5000;
                xForm.eM22 = (FLOAT)  0.8660;
                xForm.eDx  = (FLOAT)  0.0;
                xForm.eDy  = (FLOAT)  0.0;
                
                SetWorldTransform(hDC, &xForm);
                break;
                
            case SHEAR:      //Shear along the x-axis with a
                             //proportionality constant of 1.0
                xForm.eM11 = (FLOAT) 1.0;
                xForm.eM12 = (FLOAT) 1.0;
                xForm.eM21 = (FLOAT) 0.0;
                xForm.eM22 = (FLOAT) 1.0;
                xForm.eDx  = (FLOAT) 0.0;
                xForm.eDy  = (FLOAT) 0.0;
                
                SetWorldTransform(hDC, &xForm);
                break;
                
            case REFLECT:   //Reflect about a horizontal axis
                xForm.eM11 = (FLOAT) 1.0;
                xForm.eM12 = (FLOAT) 0.0;
                xForm.eM21 = (FLOAT) 0.0;
                xForm.eM22 = (FLOAT) -1.0;
                xForm.eDx  = (FLOAT) 0.0;
                xForm.eDy  = (FLOAT) 0.0;
                
                SetWorldTransform(hDC, &xForm);
                break;
                
            case NORMAL:   //set the unity transformation
                xForm.eM11 = (FLOAT) 1.0;
                xForm.eM12 = (FLOAT) 0.0;
                xForm.eM21 = (FLOAT) 0.0;
                xForm.eM22 = (FLOAT) 1.0;
                xForm.eDx  = (FLOAT) 0.0;
                xForm.eDy  = (FLOAT) 0.0;
                
                SetWorldTransform(hDC, &xForm);
                break;
        }
        
        //fond the midpoint of the client area.
        GetClientRect(hWnd, (LPRECT)&rect);
        DPtoLP(hDC, (LPPOINT)&rect, 2);
        
        //Set a hollow brush.
        SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));
        
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
