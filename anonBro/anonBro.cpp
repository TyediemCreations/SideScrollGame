/*

CS 349- Assignment 1
	John Speare
	 20393111
    September 27-29, 2013

*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <sstream>
/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "worldFunctions.cpp"

using namespace std;

const int Border = 5;
const int BufferSize = 10;


// Function to put out a message on error and exits
void error( string str ) {
  cerr << str << endl;
  exit(0);
}

//Converts player score to a string so that it may be drawn to the window
string intToString(int x){
	string answer="";
	string answerc="";
	if (x==0) return answer+((char) (0+48));
	while(x>0){
		answerc += (char) (x%10)+48;
		x = x/10;
	}
	int len = answerc.length();
	for (int i=0;i<len;i++){
		answer+=answerc[len-i-1];
	}
	return answer;
}



// Initialize X and create a window
void initX(int argc, char *argv[], XInfo &xInfo) {

	XSizeHints hints;
	unsigned long white, black;

       /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/	
	xInfo.display = XOpenDisplay( "" );
	if ( !xInfo.display )	{
		error( "Can't open display." );
	}
	
       /*
	* Find out some things about the display you're using.
	*/
	xInfo.screen = DefaultScreen( xInfo.display );

	white = XWhitePixel( xInfo.display, xInfo.screen );
	black = XBlackPixel( xInfo.display, xInfo.screen );

	hints.x = 100;
	hints.y = 100;
	xInfo.width = 800;
	xInfo.height = 600;
	hints.flags = PPosition | PSize;

	xInfo.window = XCreateSimpleWindow( 
		xInfo.display,				// display where window appears
		DefaultRootWindow( xInfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		xInfo.width, xInfo.height,	// size of the window
		5,						// width of window's border
		black,						// window border colour
		white );					// window background colour
		
	XSetStandardProperties(
		xInfo.display,		// display containing the window
		xInfo.window,		// window whose properties are set
		"Legally distinct from Mario",			// window's title
		"SD",				// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window


	// an array of graphics contexts to demo
	
	/* 
	 * Create 3 Graphics Contexts
	 */
	int i = 0;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	i = 1;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     4, LineSolid, CapRound, JoinMiter);

	i = 2;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	/*
    	 * Tell the window manager what input you want.
   	 */
    	XSelectInput( xInfo.display, xInfo.window,
                ButtonPressMask | KeyPressMask | KeyReleaseMask| ExposureMask | StructureNotifyMask);
	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xInfo.display, xInfo.window );
	
	XFlush(xInfo.display);
	sleep(1);	// let server get set up before sending drawing commands
}



//Handles a resizing of the screen. Returns false if the screen is 'too small' (i.e., below 800 wide or 600 high), true otherwise.
bool handleResize(XInfo &xinfo, XEvent &event){
	XConfigureEvent xce = event.xconfigure;
	if (xce.width != xinfo.width || xce.height != xinfo.height){
		xinfo.width = xce.width;
		xinfo.height = xce.height;
	}
	if (xinfo.width < 800 || xinfo.height < 600) return false;
	return true;
}

unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

//Eventloop. Handles events in a looping structure.
void eventLoop(XInfo &xinfo, int MarioMovementSpeed, int MarioJumpingSpeed, int sunMovementSpeed, int FPS){
	Display  *display = xinfo.display;
	Window   win = xinfo.window;

	GC       gc = xinfo.gc[0];

	int depth = DefaultDepth(display, DefaultScreen(display));
	
	Pixmap pixmap = XCreatePixmap(display,win,800,600,depth);	//A relic from when double-buffering was maybe going to be implemented. All functions requiring pixmap as a parameter leave pixmap unused, as all drawing functions instead draw directly to the window 
	 
	XEvent event;
	KeySym key;
	unsigned long lastRepaint = 0;

    	char text[BufferSize];
	string strScore;
	bool paused = true;
	bool ready = false;
	bool gameOver = false;
	bool notTooSmall = true;
	bool levelMove = false;
	bool movingLeft = false,movingRight=false;
	int i, displaceX=0, displaceY=0;
	int score,totalScore;

	block blocks[48];
	block blocksTemp[48];
	sunMoon theSun;
	notMario Mario;
	coin coins;
	flower flowers;
	mushroom mushrooms;
	spike spikes;
	Bill bBill;
	endFlag flag;

	world theWorld(blocks, blocksTemp, Mario, theSun, coins, flowers, mushrooms, spikes, bBill, MarioMovementSpeed, MarioJumpingSpeed, sunMovementSpeed, flag);	
	
	theWorld.init();

    	while( true ) {
		if (XPending(xinfo.display)>0) {  
			XNextEvent( xinfo.display, &event );
 			switch( event.type ) {
      			
       			// Clear the window on expose events.
            		case Expose:
				XClearWindow(xinfo.display, xinfo.window);
        		break;
			case ConfigureNotify:
				notTooSmall = handleResize(xinfo, event);
			break;
			case KeyRelease :
				i = XLookupString( (XKeyEvent *)&event, text, BufferSize, &key, 0 );
				

				
				if ((key >= XK_Left) && (key <= XK_Down)) {
					movingLeft = false;
					movingRight = false;
				}	
			break;
        		case KeyPress:	//such that a relevant key press is one of: SPACE (start/pause), q (quit), arrow keys (moves not-Mario), j (not-Mario jumps)
			
        			i = XLookupString( (XKeyEvent *)&event, text, BufferSize, &key, 0 );
        			if ( i == 1 && text[0] == 'q' ){   
					error( "Terminated normally." );
					XFreePixmap(display,pixmap);
            				XCloseDisplay(xinfo.display);
        			}
				if ( i == 1 && text[0] == 'j' &&!paused && !gameOver && !Mario.hurting() && !levelMove){ 
					Mario.newJump();
        			}
				else if (key==XK_space){
					if (paused) {
						paused = false;
						if (gameOver){	//game is restarted if unpaused after a game-over
							gameOver=false;

							theWorld.init();
						}
					}
					else paused = true;
				}
				if ((key >= XK_Left) && (key <= XK_Down)&&!paused && !gameOver && !Mario.hurting() && !levelMove){
					if (key == XK_Left){
						movingLeft=true;
					}
					else if (key == XK_Right){
						movingRight=true;
					}
					else Mario.moveOther();
				}
        		break;

			
			}
		}
		if (movingLeft) Mario.moveLeft();
		if (movingRight) Mario.moveRight();		

		unsigned long end = now();
		if (end - lastRepaint > 1000000/FPS) {
			XClearWindow(xinfo.display, xinfo.window);
			if (!notTooSmall){	//screen is too small, this information is displayed to the player
				XDrawString(display, win, gc, xinfo.width/2, xinfo.height/2, "too small",9);
			}
			if (paused){
				drawPaused(xinfo, xinfo.gc[0], xinfo.gc[2]);
			}
			else{

				levelMove = theWorld.update(gameOver);
				theWorld.draw(pixmap, xinfo);			
				
				score = Mario.getScore(0);
				totalScore = Mario.getScore(1);
				if (score < 0) {score=0; gameOver=true; Mario.gameOver();}	//if not-Mario's score goes beneath 0, it is game over
				if (notTooSmall){
					displaceX = (xinfo.width-800)/2;
					displaceY = (xinfo.height-600)/2;
				}
				strScore = intToString(score);
				XDrawString(display, win, gc, 700+displaceX, 30+displaceY, "Score: ",7);
				XDrawString(display, win, gc, 750+displaceX, 30+displaceY, strScore.c_str(),strScore.length());//print player's temporary score
				strScore = intToString(totalScore);
				XDrawString(display, win, gc, 50+displaceX, 30+displaceY, "Total Score: ",13);
				XDrawString(display, win, gc, 130+displaceX, 30+displaceY, strScore.c_str(),strScore.length());//print player's total score
			}
			lastRepaint = now();
		}
		if (XPending(xinfo.display) == 0) {	//event loop sleeps briefly while there are no pending requests
			usleep(1000000/FPS - (end - lastRepaint));
		}
    	}
}

/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */

int main ( int argc, char *argv[] ) {
	srand ( time(NULL) );
	XInfo xInfo;
	initX(argc, argv, xInfo);

	int MarioMovementSpeed=5;
	int MarioJumpingSpeed=5;
	int sunMovementSpeed=1;
	int frameRate=30;

	if (argc > 1){	//get values from the command line
		MarioMovementSpeed=atoi(argv[1]);
		if (argc>2) MarioJumpingSpeed=atoi(argv[2]);
		if (argc>3) sunMovementSpeed=atoi(argv[3]);
		if (argc>4) frameRate=atoi(argv[4]);
	}
	
	eventLoop(xInfo, MarioMovementSpeed, MarioJumpingSpeed, sunMovementSpeed, frameRate);
}

