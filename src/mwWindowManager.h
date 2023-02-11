// mwWindowManager.h
#ifndef mwWindowManager_H
#define mwWindowManager_H
#include "mwWindow.h"

#define NUM_MW 10

class mwWindowManager
{
   public:

   mwWindow mW[NUM_MW];

   int level_editor_mode;

   int active;


   void initialize(int edit_level);
   void set_windows(int mode);
   void loop(int edit_level);

   void get_block_position_on_map(void);
   void process_scrolledge(void);
   void show_level_buffer_block_rect(int x1, int y1, int x2, int y2, int color, const char * text);
   void get_new_box(void);
   void process_mouse(void);
   void process_keypress(void);

   void redraw_level_editor_background(int mode);
   int redraw_level_editor_background(void);

   void set_focus(int n);
   int is_mouse_on_any_window(void);
   int get_max_layer(void);
   int cycle_windows(int draw_only);

};
extern mwWindowManager mwWM;
#endif





























