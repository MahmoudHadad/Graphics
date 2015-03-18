#include <windows.h>
#include <Windowsx.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include "shape.h"
#include "circle.h"

using namespace std;

#define Cartesian 1
#define Parametric 2
#define MidPoint 3
#define CartesianCircle 4  
#define PolarCircle 5  
#define Save 6 
#define Load 7
#define Exit 7
void loadVector (HDC hdc, COLORREF color);
void saveVector ();
void drowLineMidPoint(int x1,int y1,int x2,int y2,COLORREF color,HDC hdc);
void drowLineParametric(int x1,int y1,int x2,int y2,COLORREF color,HDC hdc);
void drowLineCartesian (int x1,int y1,int x2,int y2,COLORREF color,HDC hdc);
int Max(int a,int s);

/* This is where all the input to the window goes to */
int x_1 =10;
int y_1 = 10;
int status = Cartesian;
bool firstClick = true;
vector <Shape>allShapes;

//////////////////////////////////////////////////////////////////////////////////////


LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
    COLORREF color =  RGB(0,0,0);
    int xPos;
    int yPos;
        
	switch(Message) {

		case WM_CREATE: 
		{
		
			HMENU hMenubar = CreateMenu();
			HMENU hLine = CreateMenu();
			HMENU hCircle = CreateMenu();
			HMENU hFile = CreateMenu();
				
			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFile, "File");
			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hLine, "Line");           
			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hCircle, "Circle");
			
			AppendMenu(hFile, MF_STRING, Save, "Save");
			AppendMenu(hFile, MF_STRING, Load, "Load");
			AppendMenu(hFile, MF_STRING, Exit, "Exit");
			
			AppendMenu(hLine, MF_STRING, Cartesian, "Cartesian");
			AppendMenu(hLine, MF_STRING, Parametric, "Parametric");
			AppendMenu(hLine, MF_STRING, MidPoint, "MidPoint");
			               
			AppendMenu(hCircle, MF_STRING, CartesianCircle, "CartesianCircle");
			AppendMenu(hCircle, MF_STRING, PolarCircle, "PolarCircle");
			                
			
			SetMenu(hwnd, hMenubar);
			break;
		}
		/////////////////////////
		case WM_COMMAND:{ 
			// File
			if(LOWORD(wParam) == Save)
			{
				firstClick = true;
				saveVector();
			}
			 
			else if(LOWORD(wParam) == Load)
			{
				firstClick = true;
				hdc = GetDC(hwnd);
	            
				loadVector (hdc, color);
				ReleaseDC(hwnd,hdc);
			}
			else if(LOWORD(wParam) == Exit)
			{
				
			}
			//////////////////////////////////////////
			// Line
			else if(LOWORD(wParam) == Cartesian)
			{
				firstClick = true;
				status = Cartesian;
			}
			
			else if(LOWORD(wParam) == Parametric)
			{
				firstClick = true;
				status = Parametric;
			}
			else if(LOWORD(wParam) == MidPoint)
			{
				firstClick = true;
				status = MidPoint;
			}
			////////////////////////////////////////////
			// Circle
			else if(LOWORD(wParam) == CartesianCircle)
			{
				firstClick = true;
				status = CartesianCircle;
			}
			
			else if(LOWORD(wParam) == PolarCircle)
			{
				firstClick = true;
				status = PolarCircle;
			}
			
			
			
			// add new actions here
			
			break;
		}
		/////////////////////////////////////
		case WM_LBUTTONDOWN:
        	
		   	 if(firstClick)
		   	 {
		   	 	x_1 = GET_X_LPARAM(lParam); 
				y_1 = GET_Y_LPARAM(lParam);
		   	 	firstClick = false;
		   	 }
		   	 else
		   	 {
		   	 	// add line into vector 
		   	 
		   	 	xPos = GET_X_LPARAM(lParam); 
				yPos = GET_Y_LPARAM(lParam);
	            hdc = GetDC(hwnd);            
		   	 	
	            // check alogorithm to be used 
	            if(status == Cartesian)
	            {
	            	drowLineCartesian(x_1, y_1, xPos, yPos, color, hdc);
	            }
	            else if(status == Parametric)
	            {
	            	drowLineParametric(x_1, y_1, xPos, yPos, color, hdc);
	            }
				else if	(status == MidPoint)
	            {
	            	drowLineMidPoint(x_1, y_1, xPos, yPos, color, hdc);
	            }
	            
	            else if(status == CartesianCircle)
	            {
	            	Circle c (status, x_1, y_1, xPos, yPos) ;
	            	c.cartesian( color, hdc);
	            	allShapes.push_back(c);
	            }
	            
	            else if(status == PolarCircle)
	            {
	            	Circle c (status, x_1, y_1, xPos, yPos) ;
	            	c.polar( color, hdc);
	            	allShapes.push_back(c);
	            }
	            
	            // and so on
	            ReleaseDC(hwnd,hdc);
	            firstClick = true;
		   	 }
             break;

		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG Msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		1200, /* width */
		1200, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&Msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&Msg); /* Translate key codes to chars if present */
		DispatchMessage(&Msg); /* Send it to WndProc */
	}
	return Msg.wParam;
}

 ////////////////////////////////////////////
 int Max(int a,int s){
    if(a>s) return a;
    return s;
}

void drowLineCartesian (int x1,int y1,int x2,int y2,COLORREF color,HDC hdc){
     
     int dX = x2 - x1, dY = y2 - y1;
     float m = (float) dY/dX;
     
	 if (abs (m) <1)
     {
     	if(x2<x1){
	     swap(x1,x2);
	     swap(y1,y2);
    	}
    	
     	float x, y = y1; 
     	float  c;
     	x = x1;
     	c = y1 - (m*x1);
     	while (x<=x2)
     	{
     		y = y + (m);
		     		
     		SetPixel(hdc,round(x),(y),color);
     		x++;
     	}
     }
     else 
     {
     	if(y2<y1){
	     swap(x1,x2);
	     swap(y1,y2);
    	}
    	
    	float  x = x1, y; 
     	
    	y = y1;
    	while(y<y2)
    	{
    		x = x + (1/m);
    		SetPixel(hdc, round(x),(y),color);
    		y++;
    	}
    	
     }
}
/////////////////
void drowLineParametric(int x1,int y1,int x2,int y2,COLORREF color,HDC hdc){
     if(x2<x1){
	     swap(x1,x2);
	     swap(y1,y2);
     }
     int dx=x2-x1;
     int dy=y2-y1;
     int  x=x1;
     int  y=y1;
     float ro=Max(dx,dy);
     ro=1/ro;
  
     for (float i =0;i<1;i+=ro){
         
         x= x1 + int(i*dx);
         y= y1 + int(i*dy);
         
         SetPixel(hdc,x,y,color);
         }
     
}
///////////////////////////
void drowLineMidPoint(int x1,int y1,int x2,int y2,COLORREF color,HDC hdc){
	
	 if(x2<x1){
	     swap(x1,x2);
	     swap(y1,y2);
     }
	int x = x1;
	int y = y1;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int d;
	int change1, change2;
	if(y1<=y2)
	{
		d= dx - (2*dy);
		change1 = 2*(dx - dy);
		change2 = -2 * dy;
	}
	else
	{
		d= -1*dx - (2*dy);
		change1 = -2*(dx + dy);
		change2 = -2 * dy;
	}
	SetPixel(hdc,x,y,color);
	int updateY ;
	if(y2>y1)
		updateY = 1;
	else
		updateY = -1;	
	while(x <= x2)
	{
		x = x+1;
		if(d<0)
		{
			y = y + updateY;
			d = d+change1;
		}
		else d = d+change2;
		
		SetPixel(hdc,x,y,color);
	}
	
}
//////////////////////////////////////////////////////////////////////////////////////  
void saveVector ()
{
	fstream outFile("save.txt", ios::out );
	
	for(int i=0;i<allShapes.size();i++)
	{
		outFile <<allShapes[i].algorithm <<" "<< allShapes[i].x_s <<" "<< allShapes[i].y_s <<" "<< 
				allShapes[i].x2 <<" "<< allShapes[i].y2 <<endl;
	}
	
	outFile.close();
}
////////////////////
void loadVector (HDC hdc, COLORREF color )
{
	fstream inFile("save.txt", ios::in );
	int algorithm;
	int x1,x2,y1,y2;
	while(inFile>>algorithm)
	{
		inFile>>x1>>y1>>x2>>y2;
		allShapes.push_back( *(new Shape(algorithm, x1, y1, x2, y2) ));
				   	 	
	    // check alogorithm to be used 
	    if(status == Cartesian)
        {
        	drowLineCartesian(x1, y1, x2, y2, color, hdc);
        }
        else if(status == Parametric)
        {
        	drowLineParametric(x1, y1, x2, y2, color, hdc);
        }
		else if	(status == MidPoint)
        {
        	drowLineMidPoint(x1, y1, x2, y2, color, hdc);
        }
        // and so on
       

	}
	
}

