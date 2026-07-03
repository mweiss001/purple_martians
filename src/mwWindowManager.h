// mwWindowManager.h
#ifndef mwWindowManager_H
#define mwWindowManager_H
#include "mwWindow.h"

#define NUM_MW 10

#define NUM_OBJ 600


struct tileSetGroup
{
   std::string name;
   int display_tile;
   bool visible;
};




class mwWindowManager
{
   public:

   mwWindow mW[NUM_MW];

   int level_editor_mode;

   int active;

   int obj_list[NUM_OBJ][3];
   int obj_filter[5][20];

   int thl[100][100]; // tile helper


   tileSetGroup tileSetGroups[32];


   int gx; // mouse position relative to scaled level background
   int gy;
   int hx;
   int hy;

   int bx1;  // selection window
   int by1;
   int bx2;
   int by2;

   void initialize(int edit_level);
   void set_windows(int mode);
   int loop(int edit_level);

   void get_block_position_on_map();
   void process_scrolledge();
   void show_level_buffer_block_rect(int x1, int y1, int x2, int y2, int color, const char * text);
   void show_level_buffer_block_rect_text(int x1, int y1, int x2, int y2);



   void get_new_box();

   bool get_new_box_with_preview();

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





























