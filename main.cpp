#include <windows.h>
#include <Windowsx.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cmath>
#include "shape.h"
#include "circle.h"
#include "line.h"

using namespace std;

#define Cartesian 1
#define Parametric 2
#define MidPoint 3
#define CartesianCircle 4  
#define PolarCircle 5  
#define Save 6 
#define Load 7
#define Exit 8
#define MidPointBresenham 9
#define FastMidPointCircle 10

void loadVector (HDC hdc, COLORREF color);
void saveVector ();



/* This is where all the input to the window goes to */
int x_1 =10;
int y_1 = 10;
int status = Cartesian;
bool firstClick = true;
vector <Shape*>allShapes;

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
			AppendMenu(hCircle, MF_STRING, MidPointBresenham, "MidPoint_Bresenham");             
			AppendMenu(hCircle, MF_STRING, FastMidPointCircle, "MidPoint_Fast");
			
			
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
				PostQuitMessage(0);
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
			
			else if(LOWORD(wParam) == MidPointBresenham)
			{
				firstClick = true;
				status = MidPointBresenham;
			}
			
			else if(LOWORD(wParam) == FastMidPointCircle)
			{
				firstClick = true;
				status = FastMidPointCircle;
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
	            //Line
	            if(status == Cartesian)
	            {
	            	Line *l = new Line(Cartesian,x_1, y_1, xPos, yPos);
	            	l->drowLineCartesian(color, hdc);
	            	allShapes.push_back(l);
	            }
	            else if(status == Parametric)
	            {
	            	Line *l = new Line (Parametric,x_1, y_1, xPos, yPos);
	            	l->drowLineParametric( color, hdc);
	            	allShapes.push_back(l);
	            }
				else if	(status == MidPoint)
	            {
	            	Line* l = new Line (MidPoint,x_1, y_1, xPos, yPos);
	            	l->drowLineMidPoint( color, hdc);
	            	allShapes.push_back(l);
	            }
	            //////////////////////////////////////////////
	            // Circle
	            else if(status == CartesianCircle)
	            {
	            	Circle* c = new Circle(status, x_1, y_1, xPos, yPos) ;
	            	c->cartesian( color, hdc);
	            	allShapes.push_back(c);
	            }
	            
	            else if(status == PolarCircle)
	            {
	            	Circle* c = new Circle(status, x_1, y_1, xPos, yPos) ;
	            	c->polar( color, hdc);
	            	allShapes.push_back(c);
	            }
	            
	            else if(status == MidPointBresenham)
	            {
	            	Circle *c  = new Circle(status, x_1, y_1, xPos, yPos) ;
	            	c->bresenham( color, hdc);
	            	allShapes.push_back(c);
	            }
	            
	            else if(status == FastMidPointCircle)
	            {
	            	Circle* c = new Circle(status, x_1, y_1, xPos, yPos) ;
	            	c->fastBresenham( color, hdc);
	            	allShapes.push_back(c);
	            }
	            
	            
	            
	          
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



//////////////////////////////////////////////////////////////////////////////////////  
void saveVector ()
{
	fstream outFile("save.txt", ios::out );
	
	for(int i=0;i<allShapes.size();i++)
	{
		allShapes[i]->save(outFile);
		//outFile<<"1";
	}
	
	outFile.close();
}
////////////////////
void loadVector (HDC hdc, COLORREF color )
{
	fstream inFile("save.txt", ios::in );
	int algorithm;
	
	while(inFile>>algorithm)
	{
		int x1,x2,y1,y2,x3,y3;
	   	 	
	    //check alogorithm to be used 
	    // line
	    if(algorithm == Cartesian)
        {
        	inFile>>x1>>y1>>x2>>y2;
        	Line *l = new Line(algorithm,x1, y1, x2, y2);
        	l->drowLineCartesian(color, hdc);
        	allShapes.push_back(l);
        	
        }
        else if(algorithm == Parametric)
        {
        	inFile>>x1>>y1>>x2>>y2;
        	Line *l = new Line(algorithm,x1, y1, x2, y2);
        	l->drowLineParametric(color, hdc);
        	allShapes.push_back(l);
        }
		else if	(algorithm == MidPoint)
        {
        	inFile>>x1>>y1>>x2>>y2;
        	Line *l = new Line(algorithm,x1, y1, x2, y2);
        	l->drowLineMidPoint(color, hdc);
        	allShapes.push_back(l);
        }
        ///////////////////////////// 
        // circle
        else if(algorithm == CartesianCircle)
        {
        	inFile>>x1>>y1>>x2>>y2;
        	Circle* c = new Circle(algorithm,x1, y1, x2, y2);
        	c->cartesian( color, hdc);
        	allShapes.push_back(c);
        }
        
        else if(algorithm == PolarCircle)
        {
        	inFile>>x1>>y1>>x2>>y2;
        	Circle* c = new Circle(algorithm,x1, y1, x2, y2);
        	c->polar( color, hdc);
        	allShapes.push_back(c);
        }
        
        else if(algorithm == MidPointBresenham)
        {
        	inFile>>x1>>y1>>x2>>y2;
        	Circle *c  = new Circle(algorithm,x1, y1, x2, y2);
        	c->bresenham( color, hdc);
        	allShapes.push_back(c);
        }
        
        else if(algorithm == FastMidPointCircle)
        {
        	inFile>>x1>>y1>>x2>>y2;
        	Circle* c = new Circle(algorithm,x1, y1, x2, y2);
        	c->fastBresenham( color, hdc);
        	allShapes.push_back(c);
        }

	}
	
}

