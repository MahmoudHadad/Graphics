#ifndef LINE_H
#define LINE_H
#include <fstream>
#include "shape.h"

using namespace std;
class Line : public Shape
{
	public:
	Line(int algorithm, int x_s, int y_s, int x2, int y2):Shape(algorithm, x_s, y_s, x2, y2)
		{
		}
	
	void drowLineCartesian (COLORREF color,HDC hdc){
     
     int dX = x2 - x_s, dY = y2 - y_s;
     float m = (float) dY/dX;
     
	 if (abs (m) <1)
     {
     	if(x2<x_s){
	     swap(x_s,x2);
	     swap(y_s,y2);
    	}
    	
     	float x, y = y_s; 
     	float  c;
     	x = x_s;
     	c = y_s - (m*x_s);
     	while (x<=x2)
     	{
     		y = y + (m);
		     		
     		SetPixel(hdc,round(x),(y),color);
     		x++;
     	}
     }
     else 
     {
     	if(y2<y_s){
	     swap(x_s,x2);
	     swap(y_s,y2);
    	}
    	
    	float  x = x_s, y; 
     	
    	y = y_s;
    	while(y<y2)
    	{
    		x = x + (1/m);
    		SetPixel(hdc, round(x),(y),color);
    		y++;
    	}
    	
     }
	}
	/////////////////
	void drowLineParametric(COLORREF color,HDC hdc){
	     if(x2<x_s){
		     swap(x_s,x2);
		     swap(y_s,y2);
	     }
	     int dx=x2-x_s;
	     int dy=y2-y_s;
	     int  x=x_s;
	     int  y=y_s;
	     float ro=max(dx,dy);
	     ro=1/ro;
	  
	     for (float i =0;i<1;i+=ro){
	         
	         x= x_s + int(i*dx);
	         y= y_s + int(i*dy);
	         
	         SetPixel(hdc,x,y,color);
	         }
	     
	}
	///////////////////////////
	void drowLineMidPoint(COLORREF color,HDC hdc){
		
		 if(x2<x_s){
		     swap(x_s,x2);
		     swap(y_s,y2);
	     }
		int x = x_s;
		int y = y_s;
		int dx = x2 - x_s;
		int dy = y2 - y_s;
		int d;
		int change1, change2;
		if(y_s<=y2)
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
		if(y2>y_s)
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
	///////////////////////////////////////////////////////
	void save(fstream &outFile)
	{
		outFile <<algorithm <<" "<<x_s <<" "<< y_s <<" "<< 
			x2 <<" "<< y2 <<endl;
	}
	
};

#endif
