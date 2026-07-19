// mwWindowManager.h
#ifndef mwWindowManager_H
#define mwWindowManager_H
#include "mwWindow.h"
#include "mwRect.h"

#define NUM_MW 10


class mwWindowManager
{
   public:

   void init();

   mwWindow mW[NUM_MW];

   int active;

   int level_editor_mode;


   int gx; // mouse position relative to scaled level background (0-99)
   int gy;

   int hx; // mouse position relative to scaled level background (0-1999)
   int hy;

   mwRect<int> selection_rect =  mwRect<int>::fromX1Y1WH(0,0,0,0);


   void initialize(int edit_level);
   void initialize_windows();

   void set_level_editor_mode(int mode);

   int loop(int edit_level);

   void get_block_position_on_map();
   void process_scrolledge();


   void show_level_buffer_block_rect(mwRect<int> rect, int color, const char * text);
   void show_level_buffer_block_rect_text(mwRect<int> rect);
   bool get_new_box(const char* text, bool preview = false);


   void process_mouse();
   void process_keypress();

   void redraw_level_editor_background(int mode);
   int redraw_level_editor_background();


   void set_focus(int n);
   int is_mouse_on_any_window();
   int get_max_layer();
   int cycle_windows(int draw_only);

   void save_mW();
   void load_mW();

};

extern mwWindowManager mWM;
#endif





























