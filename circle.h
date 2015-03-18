#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"


class Circle : public Shape
{
	public:
		double R = 0;
		Circle(int algorithm, int xc, int yc, int x2, int y2):Shape(algorithm, xc, yc, x2, y2)
		{
			R = (x_s - x2) * (x_s - x2) + (y_s - y2) * (y_s - y2);
			R = sqrt(R);
		}
		
		void draw8Points(int x1,int y1,int x2,int y2,COLORREF color,HDC hdc)
		{
			SetPixel(hdc, x1+x2, y1+y2, color);
			SetPixel(hdc, x1+x2, y1-y2, color);
			SetPixel(hdc, x1-x2, y1+y2, color);
			SetPixel(hdc, x1-x2, y1-y2, color);
			
			SetPixel(hdc, x1+y2, y1+x2, color);
			SetPixel(hdc, x1+y2, y1-x2, color);
			SetPixel(hdc, x1-y2, y1+x2, color);
			SetPixel(hdc, x1-y2, y1-x2, color);
		}
		
		///////////////////////////////////////////////////////////
		void cartesian(COLORREF color,HDC hdc)
		{
			color = RGB(255,0,0);
			double slope = abs(x2) / abs(y2);	
			int x=0;
			double y = R;
			
			draw8Points(x_s, y_s, x2, y2, color, hdc);
			while(x<y)
			{
				x++;
				y = sqrt ((R*R) - (x*x));	
				draw8Points(x_s, y_s, x, y, color, hdc);
			}	
			
		}
		////////////////////////////////////////////////////////////
		
		void polar(COLORREF color,HDC hdc)
		{
			color = RGB(0,255,0);
			int x = R, y=0;
			draw8Points(x_s, y_s, x, y, color, hdc);
			
			double dtheta = 1/R, theta = 0;
			
			while(x>y)
			{
				theta += dtheta;
				x = R * cos(theta);
				y = R * sin(theta);
				draw8Points(x_s, y_s, x, y, color, hdc);		
			}
		}
};

#endif
