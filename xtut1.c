/* first include the standard headers that we're likely to need */
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char ** argv){
	int screen_num, width, height,originx,originy;
	int quality = atoi(argv[1]);
	unsigned long background, border;
	Window win;
	XEvent ev;
	Display *dpy;
GC pen;
	XGCValues values;
	Colormap cmap;
	XColor xc, xc2;
	/* First connect to the display server */
	dpy = XOpenDisplay(NULL);
	if (!dpy) {fprintf(stderr, "unable to connect to display\n");return 7;}

	/* these are macros that pull useful data out of the display object */
	/* we use these bits of info enough to want them in their own variables */
	screen_num = DefaultScreen(dpy);
	background = BlackPixel(dpy, screen_num);
	border = WhitePixel(dpy, screen_num);

	width = 500; /* start with a small window */
	height = 500;
	originx = 0;
	originy = 0;

	cmap = DefaultColormap(dpy, screen_num);
	XAllocNamedColor(dpy, cmap, "Black", &xc, &xc2);
	background = xc.pixel;
	XAllocNamedColor(dpy, cmap, "LightGreen", &xc, &xc2);
	border = xc.pixel;
	XAllocNamedColor(dpy, cmap, "Red", &xc, &xc2);
	values.foreground = xc.pixel;

	win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), /* display, parent */
		40,40, /* x, y: the window manager will place the window elsewhere */
		1000, 1000, /* width, height */
		2, border, /* border width & colour, unless you have a window manager */
		background); /* background colour */
	/* create the pen to draw lines with */
//	values.foreground = WhitePixel(dpy, screen_num);





	//values.line_width = 1;
	//values.line_style = LineSolid;
	pen = XCreateGC(dpy, win, GCForeground/*|GCLineWidth|GCLineStyle*/,&values);

	/* tell the display server what kind of events we would like to see */
	XSelectInput(dpy, win, ButtonPressMask|StructureNotifyMask|ExposureMask );

	/* okay, put the window on the screen, please */
	XMapWindow(dpy, win);

	/* as each event that we asked about occurs, we respond.  In this
	 * case we note if the window's shape changed, and exit if a button
	 * is pressed inside the window */
/*	while(1){
		XNextEvent(dpy, &ev);
		switch(ev.type){
		case ConfigureNotify:
			if (width != ev.xconfigure.width
					|| height != ev.xconfigure.height) {
				width = ev.xconfigure.width;
				height = ev.xconfigure.height;
				printf("Size changed to: %d by %d\n", width, height);
			}
			break;
		case ButtonPress:
			//goto exit; 
		break;
			
		}
	}*/
		int count=0;
		int max=100;
		while(1){
		XNextEvent(dpy, &ev);
		switch(ev.type){
		case Expose:
		{
		int row=0;

		for (; row < height; row=row+quality) {
			int col=0;
    for (; col < width; col=col+quality) {
        long double c_re = (col - width/2.0 + position)*resolution/width;// default position 30000 || 0; resolution 0.0125 || 4 
        long double c_im = (row - height/2.0 + position)*resolution/width;
        long double x = 0, y = 0;
        int iteration = 0;
        while (x*x+y*y <= 4 && iteration < max) {
            long double x_new = x*x - y*y + c_re;
            y = 2*x*y + c_im;
            x = x_new;
            iteration=iteration+1;
        }
        if (iteration < max) 
        { count=count+1;
        	XColor xcolour;

// I guess XParseColor will work here
xcolour.red = 0; xcolour.green = 0; xcolour.blue = (max-iteration)*65535/10;
xcolour.flags = DoRed | DoGreen | DoBlue;
XAllocColor(dpy, cmap, &xcolour);

XSetForeground(dpy, pen, xcolour.pixel);
        	XDrawPoint(dpy,win,pen,col+originx, row+originy);
        }

        	XFlush(dpy);
        //else putpixel(col, row, black);
    }
}}
		// printf("%d\n",count );
			//XDrawLine(dpy, win, pen, 0, 0, width, height);
			//XDrawLine(dpy, win, pen, width, 0, 0, height);
			break;
		case ConfigureNotify:
			if (width != ev.xconfigure.width
					|| height != ev.xconfigure.height) {
				width = ev.xconfigure.width;
				height = ev.xconfigure.height;
				XClearWindow(dpy, ev.xany.window);
				printf("Size changed to: %d by %d\n", width, height);
			}
			break;
		case ButtonPress:
			goto exit;
			return 0;
		}
	}
	exit:
	XCloseDisplay(dpy);

	return 0;
}
