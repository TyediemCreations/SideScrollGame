#include "XwinStructs.h"

//Various draw functions
void drawBlock(XInfo &xinfo, GC gc, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;	//center image on window
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawRectangle(display, win, gc, x, y, 50, 50);
}
void drawMarioStill(XInfo &xinfo, GC gc, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawLine(display, win, gc, x+25, y+15, x+25, y+35);	//body
	XDrawLine(display, win, gc, x+15, y+20, x+35, y+20);	//arms
	XDrawLine(display, win, gc, x+25, y+35, x+15, y+50);	//left leg
	XDrawLine(display, win, gc, x+25, y+35, x+35, y+50);	//right leg
	XDrawArc(display, win, gc, (x+25)-(10/2), (y+10)-(10/2), 10, 10, 0, 360*64); //head
}
void drawMarioLeft(XInfo &xinfo, GC gc, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawLine(display, win, gc, x+25, y+15, x+25, y+35);	//body
	XDrawLine(display, win, gc, x+25, y+20, x+15, y+15);	//top arm
	XDrawLine(display, win, gc, x+25, y+20, x+15, y+25);	//bottom arm
	XDrawLine(display, win, gc, x+25, y+35, x+15, y+50);	//left leg
	XDrawLine(display, win, gc, x+25, y+35, x+35, y+50);	//right leg
	XDrawArc(display, win, gc, (x+25)-(10/2), (y+10)-(10/2), 10, 10, 0, 360*64); //head
}
void drawMarioRight(XInfo &xinfo, GC gc, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawLine(display, win, gc, x+25, y+15, x+25, y+35);	//body
	XDrawLine(display, win, gc, x+25, y+20, x+35, y+15);	//top arm
	XDrawLine(display, win, gc, x+25, y+20, x+35, y+25);	//bottom arm
	XDrawLine(display, win, gc, x+25, y+35, x+15, y+50);	//left leg
	XDrawLine(display, win, gc, x+25, y+35, x+35, y+50);	//right leg
	XDrawArc(display, win, gc, (x+25)-(10/2), (y+10)-(10/2), 10, 10, 0, 360*64); //head
}
void drawMarioRunLeft(XInfo &xinfo, GC gc, int x, int y, int counter){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawLine(display, win, gc, x+25, y+15, x+25, y+35);	//body
	XDrawLine(display, win, gc, x+25, y+20, x+15, y+15);	//top arm
	XDrawLine(display, win, gc, x+25, y+20, x+15, y+25);	//bottom arm
	
	if (counter){
		XDrawLine(display, win, gc, x+25, y+35, x+20, y+50);	//left leg
		XDrawLine(display, win, gc, x+25, y+35, x+40, y+47);	//right leg
	}else {
		XDrawLine(display, win, gc, x+25, y+35, x+10, y+47);	//left leg
		XDrawLine(display, win, gc, x+25, y+35, x+30, y+50);	//right leg
	}
	XDrawArc(display, win, gc, (x+25)-(10/2), (y+10)-(10/2), 10, 10, 0, 360*64); //head
}
void drawMarioRunRight(XInfo &xinfo, GC gc, int x, int y, int counter){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawLine(display, win, gc, x+25, y+15, x+25, y+35);	//body
	XDrawLine(display, win, gc, x+25, y+20, x+35, y+15);	//top arm
	XDrawLine(display, win, gc, x+25, y+20, x+35, y+25);	//bottom arm
	
	if (counter){
		XDrawLine(display, win, gc, x+25, y+35, x+10, y+47);	//left leg
		XDrawLine(display, win, gc, x+25, y+35, x+30, y+50);	//right leg
	}else {
		XDrawLine(display, win, gc, x+25, y+35, x+20, y+50);	//left leg
		XDrawLine(display, win, gc, x+25, y+35, x+40, y+47);	//right leg
	}
	XDrawArc(display, win, gc, (x+25)-(10/2), (y+10)-(10/2), 10, 10, 0, 360*64); //head
}
void drawSun(XInfo &xinfo, GC gc, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawArc(display, win, gc, (x+50)-(70/2), (y+50)-(70/2), 70, 70, 0, 360*64);	//sun body
	XDrawLine(display, win, gc, x+50, y+10, x+50, y);	//top beam
	XDrawLine(display, win, gc, x+50, y+90, x+50, y+100);	//bottom beam
	XDrawLine(display, win, gc, x+90, y+26, x+100, y+20);	//upper right beam
	XDrawLine(display, win, gc, x+90, y+74, x+100, y+80);	//bottom right beam
	XDrawLine(display, win, gc, x+10, y+26, x, y+20);	//upper left beam
	XDrawLine(display, win, gc, x+10, y+74, x, y+80);	//bottom left beam
}
void drawMoon(XInfo &xinfo, GC gc, GC gcBlock, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawArc(display, win, gc, (x+50)-(55/2), (y+50)-(55/2), 55, 55, 0, 360*64);	//right side of the moon
	XDrawArc(display, win, gc, (x+50)-(20/2), (y+50)-(55/2), 20, 55, 0, 360*64);	//left side of the moon
	XFillRectangle(display, win, gcBlock, x, y, 50, 100);	//dark side of the moon
}
void drawCoin(XInfo &xinfo, GC gc, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawArc(display, win, gc, (x+25)-(10/2), (y+25)-(15/2), 10, 15, 0, 360*64);	//coin body
	XDrawLine(display, win, gc, x+25, y+22, x+25, y+30);	//coin inscription
	XDrawLine(display, win, gc, x+27, y+22, x+27, y+27);	//light reflection
}
void drawSpikes(XInfo &xinfo, GC gc, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawLine(display, win, gc, x+4, y+50, x+11, y+40);
	XDrawLine(display, win, gc, x+18, y+50, x+11, y+40);
	XDrawLine(display, win, gc, x+18, y+50, x+25, y+40);
	XDrawLine(display, win, gc, x+32, y+50, x+25, y+40);
	XDrawLine(display, win, gc, x+32, y+50, x+39, y+40);
	XDrawLine(display, win, gc, x+46, y+50, x+39, y+40);
}
void drawBill(XInfo &xinfo, GC gc, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawRectangle(display, win, gc, x+12.5, y+35, 25, 15);	//base
	XDrawRectangle(display, win, gc, x+20, y+25, 10, 10);	//pillar
	XDrawRectangle(display, win, gc, x, y, 50, 25);	//body

	XDrawLine(display, win, gc, x+5, y, x+5, y+25);
	XDrawLine(display, win, gc, x+45, y, x+45, y+25);
	XDrawArc(display, win, gc, (x+25)-(8/2), (y+7)-(8/2), 8, 8, 0, 360*64);
	XDrawLine(display, win, gc, x+18, y+13, x+32, y+18);
	XDrawLine(display, win, gc, x+18, y+18, x+32, y+13);
	
}
void drawBullet(XInfo &xinfo, GC gc, GC gcBlock, int x, int y, bool right){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawArc(display, win, gc, (x+25)-(40/2), (y+12.5)-(22/2), 40, 22, 0, 360*64);
	if (right){
		XFillRectangle(display, win, gcBlock, x, y, 20, 25);
		XDrawLine(display, win, gc, x+20, y+1.5, x+20, y+23.5);
	}else{
		XFillRectangle(display, win, gcBlock, x+(50-20), y, 20, 25);
		XDrawLine(display, win, gc, x+(50-20), y+1.5, x+(50-20), y+23.5);
	}
}
void drawFlower(XInfo &xinfo, GC gc, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawLine(display, win, gc, x+25, y+50, x+25, y+35);	//flower stem
	XDrawArc(display, win, gc, (x+25)-(8/2), (y+31)-(8/2), 8, 8, 0, 360*64);	//flower head
	XDrawArc(display, win, gc, (x+25)-(4/2), (y+25)-(4/2), 4, 4, 0, 360*64);
	XDrawArc(display, win, gc, (x+30)-(4/2), (y+28)-(4/2), 4, 4, 0, 360*64);
	XDrawArc(display, win, gc, (x+30)-(4/2), (y+34)-(4/2), 4, 4, 0, 360*64);
	XDrawArc(display, win, gc, (x+20)-(4/2), (y+28)-(4/2), 4, 4, 0, 360*64);
	XDrawArc(display, win, gc, (x+20)-(4/2), (y+34)-(4/2), 4, 4, 0, 360*64);
}
void drawMushroom(XInfo &xinfo, GC gc, GC gcBlock, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawArc(display, win, gc, (x+25)-(16/2), (y+38)-(16/2), 16, 16, 0, 360*64);	//mushroom head
	XFillRectangle(display, win, gcBlock, x+17, y+38, 20, 9);
	XDrawRectangle(display, win, gc, x+23, y+38, 4, 12);	//mushroom base
	XDrawLine(display, win, gc, x+17, y+38, x+33, y+38);
}
void drawFlag(XInfo &xinfo, GC gc, int x, int y){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	if (xinfo.width > 800) x = x+(xinfo.width-800)/2;
	if (xinfo.height > 600) y = y+(xinfo.height-600)/2;

	XDrawLine(display, win, gc, x+25, y, x+25, y+200);
	XDrawLine(display, win, gc, x+25, y+20, x, y+15);
	XDrawLine(display, win, gc, x+25, y+5, x, y+15);
}

//Pause screen, seen on startup.
void drawPaused(XInfo &xinfo, GC gc, GC gcBlock){
	Display *display = xinfo.display;
	Window win = xinfo.window;
	
	std::string displayString;
	int x=0, y=0;
	if (xinfo.width > 800)  x = (xinfo.width-800)/2;
	if (xinfo.height > 600) y = (xinfo.height-600)/2;

	displayString = "Super Anonymous Plumber Bro";
	XDrawString(display, win, gc, 300+x, 220+y, displayString.c_str(),displayString.length());
	displayString = "Controls:";	
	XDrawString(display, win, gc, 50+x, 250+y, displayString.c_str(),displayString.length());
	displayString = "Space Bar-      Pause/Start/Restart game";
	XDrawString(display, win, gc, 80+x, 270+y, displayString.c_str(),displayString.length());
	displayString = "Up/Down Arrow-  Face Forward";
	XDrawString(display, win, gc, 80+x, 290+y,displayString.c_str(),displayString.length());
	displayString = "Left Arrow-     Move Left";
	XDrawString(display, win, gc, 80+x, 310+y, displayString.c_str(),displayString.length());
	displayString = "Right Arrow-    Move Right";
	XDrawString(display, win, gc, 80+x, 330+y, displayString.c_str(),displayString.length());
	displayString = "j-              Jump";
	XDrawString(display, win, gc, 80+x, 350+y, displayString.c_str(), displayString.length());
	displayString = "q-              Exit game";
	XDrawString(display, win, gc, 80+x, 370+y,displayString.c_str(),displayString.length());

	for (int i=0;i<11;i++){
		drawBlock(xinfo, gc, (50*i)+100, 450);
	}

	drawMarioRunLeft(xinfo, gc, 150, 400, 1);
	drawMarioLeft(xinfo, gc, 200, 400);
	drawMarioStill(xinfo, gc, 250, 400);
	drawMarioRight(xinfo, gc, 300, 400);
	drawMarioRunRight(xinfo, gc, 350, 400, 1);

	drawSun(xinfo, gc, 150, 100);
	drawMoon(xinfo, gc, gcBlock, 450, 100);

	drawFlower(xinfo, gc, 100, 400);
	drawCoin(xinfo, gc, 400, 400);
	drawSpikes(xinfo, gc, 450, 400);
	drawBullet(xinfo, gc, gcBlock, 550, 400, true);
	drawBill(xinfo, gc, 500, 400);
	drawMushroom(xinfo, gc, gcBlock, 600, 400);
	 
}
