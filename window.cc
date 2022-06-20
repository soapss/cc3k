#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/xpm.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "window.h"

using namespace std;

Xwindow::Xwindow(int width, int height) {

  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapRaised(d, w);

  // process window close event through event handler so XNextEvent does not fail
  Atom del_window = XInternAtom(d, "WM_DELETE_WINDOW", 0);
  XSetWMProtocols(d, w, &del_window, 1);

  Pixmap pix = XCreatePixmap(d,w,width,
        height,DefaultDepth(d,DefaultScreen(d)));
  gc = XCreateGC(d, pix, 0,(XGCValues *)0);

  XFlush(d);
  XFlush(d);

  // Set up colours.
  XColor xcolour;
  Colormap cmap;
  char color_vals[10][10]={"white", "black", "red", "green", "blue", "cyan", "yellow", "magenta", "orange", "brown"};

  cmap=DefaultColormap(d,DefaultScreen(d));
  for(int i=0; i < 5; ++i) {
      XParseColor(d,cmap,color_vals[i],&xcolour);
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }

  XSetForeground(d,gc,colours[Black]);

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d,True);

  if (XpmReadFileToImage (d, "wall.xpm", &wallImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "passageway.xpm", &passageImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "pc.xpm", &pcImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "potion.xpm", &potionImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "stairway.xpm", &stairwayImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "floor.xpm", &floorImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "door.xpm", &doorImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "gold.xpm", &goldImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "space.xpm", &spaceImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }

  if (XpmReadFileToImage (d, "dragon.xpm", &dragonImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "dwarf.xpm", &dwarfImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "elf.xpm", &elfImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "orc.xpm", &orcImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  
  }
  if (XpmReadFileToImage (d, "human.xpm", &humanImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  
  }
  if (XpmReadFileToImage (d, "halfling.xpm", &halflingImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "merchant.xpm", &merchantImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  if (XpmReadFileToImage (d, "pathfinder.xpm", &pathfinderImg, NULL, NULL)) {
      cerr << "Error reading file" << endl;
      exit (1);
  }
  usleep(100);
}

Xwindow::~Xwindow() {
  XFreeGC(d, gc);
  // Destroy all the images read into memory
  XDestroyImage(pcImg);
  XDestroyImage(floorImg);
  XDestroyImage(potionImg);
  XDestroyImage(passageImg);
  XDestroyImage(stairwayImg);
  XDestroyImage(wallImg);
  XDestroyImage(doorImg);
  XDestroyImage(goldImg);
  XDestroyImage(spaceImg);

  XCloseDisplay(d);
}

Window & Xwindow::getWindow() { return w; }
Display * Xwindow::getDisplay() { return d; }

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {


  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawString(int x, int y, string msg) {
  XDrawString(d, w, DefaultGC(d, s), x, y, msg.c_str(), msg.length());
}

void Xwindow::drawImage(XImage *img, int x, int y) {
      XPutImage(d, w, gc, img, 0, 0,
                x,
                y,
                img->width, img->height );

}

