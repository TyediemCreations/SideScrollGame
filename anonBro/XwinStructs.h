// Handy struct to save display, window, and screen
struct XInfo {
	Display	 *display;
	int screen;
	Window	 window;
	GC  gc[3];
 	int width;
	int height;
};
