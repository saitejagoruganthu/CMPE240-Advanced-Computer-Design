/*
===============================================================================
 Name        : 3DCube_Sphere.c
 Author      : Tirumala Saiteja Goruganthu
 SJSU ID	 : 016707210
 Course	 	 : CMPE240 - Advanced Computer Design
 Professor	 : Harry Li
 Description : This program is written to realize a cube and a half sphere. The
 	 	 	   cube is drawn using the points first plotted in world coordinate
 	 	 	   system and then using world-to-viewer transform, perspective
 	 	 	   transform, we use drawLine method to plot the cube on the LCD screen.
 	 	 	   Similarly, the half sphere is realized by drawing a circle in the X-Y
 	 	 	   plane and simultaneously drawing contours with same center, decreased
 	 	 	   radius and increasing Z value. For the 4-vertices patches, I have
 	 	 	   written a simple code to first store a selected equi-distant points on
 	 	 	   each contour and joining the points from one contour to upper contour
 	 	 	   using drawLine method.
===============================================================================
*/

#include <cr_section_macros.h>
#include <NXP/crp.h>
#include "LPC17xx.h"                        /* LPC17xx definitions */
#include "ssp.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/* Be careful with the port number and location number, because

some of the location may not exist in that port. */

#define PORT_NUM            0

uint8_t src_addr[SSP_BUFSIZE];
uint8_t dest_addr[SSP_BUFSIZE];

#define ST7735_TFTWIDTH 127
#define ST7735_TFTHEIGHT 159

#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_SLPOUT 0x11
#define ST7735_DISPON 0x29

#define swap(x, y) {x = x + y; y = x - y; x = x - y ;}

// defining color values

#define LIGHTBLUE 0x00FFE0
#define GREEN 0x00FF00
#define DARKBLUE 0x000033
#define BLACK 0x000000
#define BLUE 0x0007FF
#define RED 0xFF0000
#define MAGENTA 0x00F81F
#define WHITE 0xFFFFFF
#define PURPLE 0xCC33FF

// Self Defined Colors
#define BROWN 0xA52A2A
#define YELLOW 0xFFFE00

// Self Defined GREEN Shades
#define GREEN1 0x38761D
#define GREEN2 0x002200
#define GREEN3 0x00FC7C
#define GREEN4 0x32CD32
#define GREEN5 0x228B22
#define GREEN6 0x006400

// Self Defined RED Shades
#define RED1 0xE61C1C
#define RED2 0xEE3B3B
#define RED3 0xEF4D4D
#define RED4 0xE88080

int _height = ST7735_TFTHEIGHT;
int _width = ST7735_TFTWIDTH;

// Defining the eye co-ordinates
float Xe=200, Ye=200, Ze=200;

// Defining the focal length
float D_focal=120;

void spiwrite(uint8_t c)
{

	 int pnum = 0;

	 src_addr[0] = c;

	 SSP_SSELToggle( pnum, 0 );

	 SSPSend( pnum, (uint8_t *)src_addr, 1 );

	 SSP_SSELToggle( pnum, 1 );

}

void writecommand(uint8_t c)

{

	 LPC_GPIO0->FIOCLR |= (0x1<<3);

	 spiwrite(c);

}

void writedata(uint8_t c)

{

	 LPC_GPIO0->FIOSET |= (0x1<<3);

	 spiwrite(c);

}

void writeword(uint16_t c)

{

	 uint8_t d;

	 d = c >> 8;

	 writedata(d);

	 d = c & 0xFF;

	 writedata(d);

}

void write888(uint32_t color, uint32_t repeat)

{
	 uint8_t red, green, blue;

	 int i;

	 red = (color >> 16);

	 green = (color >> 8) & 0xFF;

	 blue = color & 0xFF;

	 for (i = 0; i< repeat; i++) {

	  writedata(red);

	  writedata(green);

	  writedata(blue);
	 }

}

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)

{
	 writecommand(ST7735_CASET);

	 writeword(x0);

	 writeword(x1);

	 writecommand(ST7735_RASET);

	 writeword(y0);

	 writeword(y1);

}

void fillrect(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)

{
	 int16_t width, height;

	 width = x1-x0+1;

	 height = y1-y0+1;

	 setAddrWindow(x0,y0,x1,y1);

	 writecommand(ST7735_RAMWR);

	 write888(color,width*height);

}

void lcddelay(int ms)

{
	 int count = 24000;
	 int i;
	 for ( i = count*ms; i > 0; i--);
}

void lcd_init()

{
	 int i;
	 printf("Welcome to my CMPE240 Assignment - 3D_Cube_and_Sphere\n");
	 // Set pins P0.16, P0.3, P0.22 as output
	 LPC_GPIO0->FIODIR |= (0x1<<16);

	 LPC_GPIO0->FIODIR |= (0x1<<3);

	 LPC_GPIO0->FIODIR |= (0x1<<22);

	 // Hardware Reset Sequence
	 LPC_GPIO0->FIOSET |= (0x1<<22);
	 lcddelay(500);

	 LPC_GPIO0->FIOCLR |= (0x1<<22);
	 lcddelay(500);

	 LPC_GPIO0->FIOSET |= (0x1<<22);
	 lcddelay(500);

	 // initialize buffers
	 for ( i = 0; i < SSP_BUFSIZE; i++ )
	 {

	   src_addr[i] = 0;
	   dest_addr[i] = 0;
	 }

	 // Take LCD display out of sleep mode
	 writecommand(ST7735_SLPOUT);
	 lcddelay(200);

	 // Turn LCD display on
	 writecommand(ST7735_DISPON);
	 lcddelay(200);

}

// Converting virtual X-Coordinate to physical X-Coordinate
int16_t xConvertToPhysical(int16_t x)
{
	x = x + (_width>>1);
	return x;
}

// Converting virtual Y-Coordinate to physical Y-Coordinate
int16_t yConvertToPhysical(int16_t y)
{
	y = (_height>>1) - y;
	return y;
}

void drawPixel(int16_t x, int16_t y, uint32_t color)

{
	// Convert Virtual to Physical
	x=xConvertToPhysical(x);
	y=yConvertToPhysical(y);

	 if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))

	 return;

	 setAddrWindow(x, y, x + 1, y + 1);

	 writecommand(ST7735_RAMWR);

	 write888(color, 1);

}

/*****************************************************************************


** Descriptions:        Draw line function

**

** parameters:           Starting point (x0,y0), Ending point(x1,y1) and color

** Returned value:        None

**

*****************************************************************************/


void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color)

{

	 int16_t slope = abs(y1 - y0) > abs(x1 - x0);

	 if (slope) {

	  swap(x0, y0);

	  swap(x1, y1);

	 }

	 if (x0 > x1) {

	  swap(x0, x1);

	  swap(y0, y1);

	 }

	 int16_t dx, dy;

	 dx = x1 - x0;

	 dy = abs(y1 - y0);

	 int16_t err = dx / 2;

	 int16_t ystep;

	 if (y0 < y1) {

	  ystep = 1;

	 }

	 else {

	  ystep = -1;

	 }

	 for (; x0 <= x1; x0++) {

	  if (slope) {

	   drawPixel(y0, x0, color);

	  }

	  else {

	   drawPixel(x0, y0, color);

	  }

	  err -= dy;

	  if (err < 0) {

	   y0 += ystep;

	   err += dx;

	  }

	 }

}

// Declare a structure for 3D
typedef struct
{
	float x_value; float y_value; float z_value;
}Pts3D;

// Declare a structure for 2D
typedef struct
{
	float x; float y;
}Pts2D;

// World to Viewer Transform method
Pts3D getWorld2Viewer(float WCS_X, float WCS_Y, float WCS_Z)
{
	Pts3D V;


	float Rho=sqrt(pow(Xe,2)+pow(Ye,2)+pow(Xe,2));

	float sPheta = Ye/sqrt(pow(Xe,2)+pow(Ye,2));
	float cPheta = Xe/sqrt(pow(Xe,2)+pow(Ye,2));
	float sPhi = sqrt(pow(Xe,2)+pow(Ye,2))/Rho;
	float cPhi = Ze/Rho;

	V.x_value = -sPheta * WCS_X + cPheta * WCS_Y;
	V.y_value = -cPheta * cPhi * WCS_X - cPhi * sPheta * WCS_Y + sPhi * WCS_Z;
	V.z_value = -sPhi * cPheta * WCS_X - sPhi * cPheta * WCS_Y - cPhi * WCS_Z + Rho;

	return V;
}

// Viewer to Perspective Transform method
Pts2D getViewer2Perspective(float V_X, float V_Y, float V_Z)
{
	Pts2D P;

	P.x=V_X*(D_focal/V_Z);
	P.y=V_Y*(D_focal/V_Z);

	return P;
}

// method to draw the cube
void drawCube()
{
	#define UpperBD 52
	#define NumOfPts 10

	typedef struct
	{
		float X[UpperBD]; float Y[UpperBD]; float Z[UpperBD];
	}pworld;

	typedef struct
	{
		float X[UpperBD]; float Y[UpperBD]; float Z[UpperBD];
	}pviewer;

	typedef struct
	{
		float X[UpperBD]; float Y[UpperBD];
	}pperspective;

	pworld WCS;
	pviewer V;
	pperspective P;
	Pts3D Vsingle;
	Pts2D Psingle;

	// Origin
	WCS.X[0]=0.0; WCS.Y[0]=0.0; WCS.Z[0]=0.0;

	// Axes
	WCS.X[1]=200.0; WCS.Y[1]=0.0; WCS.Z[1]=0.0;
	WCS.X[2]=0.0; WCS.Y[2]=200.0; WCS.Z[2]=0.0;
	WCS.X[3]=0.0; WCS.Y[3]=0.0;	WCS.Z[3]=200.0;

	// Elevate Cube along Z_w axis by 10
	WCS.X[4]=100.0; WCS.Y[4]=0; WCS.Z[4]=10.0;
	WCS.X[5]=0.0; WCS.Y[5]=100.0; WCS.Z[5]=10.0;
	WCS.X[6]=0.0; WCS.Y[6]=0.0; WCS.Z[6]=110.0;
	WCS.X[7]=100.0; WCS.Y[7]=0.0; WCS.Z[7]=110.0;
	WCS.X[8]=100.0; WCS.Y[8]=100.0; WCS.Z[8]=10.0;
	WCS.X[9]=0.0; WCS.Y[9]=100.0; WCS.Z[9]=110.0;
	WCS.X[10]=100.0; WCS.Y[10]=100.0; WCS.Z[10]=110.0;

	// World to Viewer Transform for each point
	for(int i=0;i<=NumOfPts;i++)
	{
		Vsingle = getWorld2Viewer(WCS.X[i], WCS.Y[i], WCS.Z[i]);
		V.X[i] = Vsingle.x_value;
		V.Y[i] = Vsingle.y_value;
		V.Z[i] = Vsingle.z_value;
	}

	// Viewer to perspective transform for each point
	for(int i=0;i<=NumOfPts;i++)
	{
		Psingle = getViewer2Perspective(V.X[i], V.Y[i], V.Z[i]);
		P.X[i]=Psingle.x;
		P.Y[i]=Psingle.y;
	}

	// Draw Lines for all the edges of the cube
	drawLine(P.X[0],P.Y[0],P.X[1],P.Y[1],RED);
	drawLine(P.X[0],P.Y[0],P.X[2],P.Y[2],0x00FF00);
	drawLine(P.X[0],P.Y[0],P.X[3],P.Y[3],0x0000FF);
	drawLine(P.X[7],P.Y[7],P.X[4],P.Y[4],WHITE);
	drawLine(P.X[7],P.Y[7],P.X[6],P.Y[6],WHITE);
	drawLine(P.X[7],P.Y[7],P.X[10],P.Y[10],WHITE);
	drawLine(P.X[8],P.Y[8],P.X[4],P.Y[4],WHITE);
	drawLine(P.X[8],P.Y[8],P.X[5],P.Y[5],WHITE);
	drawLine(P.X[8],P.Y[8],P.X[10],P.Y[10],WHITE);
	drawLine(P.X[9],P.Y[9],P.X[6],P.Y[6],WHITE);
	drawLine(P.X[9],P.Y[9],P.X[5],P.Y[5],WHITE);
	drawLine(P.X[9],P.Y[9],P.X[10],P.Y[10],WHITE);
}

// Method to draw the half sphere using contours
void drawSphere()
{
	#define UpperBD 360
	#define UpperPCBD 100
	#define TotalPts 360
	#define NumOfLevels 10

	typedef struct
	{
		float X[UpperBD]; float Y[UpperBD]; float Z[UpperBD];
	}pworld;

	typedef struct
	{
		float X[UpperBD]; float Y[UpperBD]; float Z[UpperBD];
	}pviewer;

	typedef struct
	{
		float X[UpperBD]; float Y[UpperBD];
	}pperspective;

	// Structure to hold the equi-distant points on each contour
	typedef struct
	{
		float X[UpperPCBD]; float Y[UpperPCBD]; float Z[UpperPCBD];
	}pcontour;

	pworld WCS;
	pviewer V;
	pperspective P;
	pcontour PC;
	Pts3D Vsingle;
	Pts2D Psingle;

	float angle_theta;
	int radius = 100;
	int kx=0, ky=0, i, level;

	for(level=0;level<=NumOfLevels-1;level++)
	{
		for(i=0;i<TotalPts;i++)
		{
			// Logic to draw a circle using angles
			angle_theta = i*3.142 /180;
			WCS.X[i] = 0 + radius*cos(angle_theta);
			WCS.Y[i] =  0 + radius*sin(angle_theta);
			WCS.Z[i] = 10*level;	// Elevate the contour using Z_w each level
		}

		// World to Viewer Transform for each point
		for(i=0;i<TotalPts;i++)
		{
			Vsingle = getWorld2Viewer(WCS.X[i], WCS.Y[i], WCS.Z[i]);
			V.X[i] = Vsingle.x_value;
			V.Y[i] = Vsingle.y_value;
			V.Z[i] = Vsingle.z_value;
		}

		// Viewer to perspective transform for each point
		for(i=0;i<TotalPts;i++)
		{
			Psingle = getViewer2Perspective(V.X[i], V.Y[i], V.Z[i]);
			P.X[i]=Psingle.x;
			P.Y[i]=Psingle.y;
			drawPixel(P.X[i], P.Y[i], WHITE);

			// Logic to store a selected number of equi-distant points on each contour into PC.
			// In this case, for each contour, 10 points are selected and later joined.
			if(i%(TotalPts/10) == 0)
			{
				PC.X[kx++] = P.X[i];
				PC.Y[ky++] = P.Y[i];
			}
		}

		radius-=10;	// decrease radius of each contour when level increases and Z_w increases
	}

	// Logic to join the equi-distant points of one contour to the next contour
	for(i=0;i<kx;i++)
	{
		if((i+10)<kx)
		drawLine(PC.X[i], PC.Y[i], PC.X[i+10], PC.Y[i+10], WHITE);
	}
}

int main (void)
{
	uint32_t pnum = 0 ;

	if ( pnum == 0 )
		SSP0Init();
	else
		 puts("Port number is not correct");

	 lcd_init();

	 fillrect(0, 0, ST7735_TFTWIDTH, ST7735_TFTHEIGHT, BLACK);

	 drawCube();

	 drawSphere();

	 return 0;
}
