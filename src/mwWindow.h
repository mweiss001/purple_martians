// mwWindow.h
#ifndef MWWINDOW_H
#define MWWINDOW_H
#include <functional>

#include "mwRect.h"

class mwWindow
{
   public:
   std::function<void()> drawFunction = nullptr;
   std::function<bool()> detectMouse = nullptr;
   std::function<void()> redrawCallback = nullptr;


   mwRect<int> rect = mwRect<int>::fromX1Y1WH(0,0,0,0);


   int min_width;
   int max_width;

   int min_height;
   int max_height;


   int index;
   int active;

   int have_focus;
   int disable_input;

   int layer;
   int moveable;
   int moving;
   int resizable;

   int color;
   char title[80];

   mwWindow(); // default constructor
   void set_title(const char*);
   void set_pos(int, int);
   void set_size(int, int);

   void check_offscreen();

   void process_mouse();

   bool detect_mouse();

   void draw(int draw_only);

};
#endif
