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

   int draw_mode;

   int color;
   int hover_highlight;

   char title[80];
   int title_text_just;


   bool enable_X_button;

   int resizable;
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


   mwWindow(); // default constructor
   void set_title(const char*);
   void set_pos(int, int);
   void set_size(int, int);

   void init(int p_index, int p_player, int p_x, int p_y, int p_w, int p_h, int p_color, const char* p_title, int p_draw_mode, int p_title_text_just, int p_title_bar_hover_highlight, bool p_enable_X_button);

   void set_resizeable(int min_w, int max_w, int min_h, int max_h);

   void check_offscreen();

   void process_mouse();

   bool detect_mouse();

   void draw(int draw_only);

};
#endif
