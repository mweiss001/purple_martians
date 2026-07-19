// mwWindow.h
#ifndef MWWINDOW_H
#define MWWINDOW_H
#include "mwRect.h"

class mwWindow
{
   public:

   mwRect<int> rect = mwRect<int>::fromX1Y1WH(0,0,0,0);

   int index;
   int active;
   int have_focus;
   int layer;
   int moveable;
   int moving;
   int resizable;
   int hidden;
   int color;
   char title[80];


   mwWindow(); // default constructor
   void set_pos(int, int);
   void set_size(int, int);
   void draw(int draw_only);
   int detect_mouse();
   void set_title(const char*);
   void process_mouse();
   void check_offscreen();
};
#endif // MWWINDOW_H
