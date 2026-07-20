#ifndef PM_MWLEVELEDITOR
#define PM_MWLEVELEDITOR
#include "mwWindowManager.h"

class mwLevelEditor
{
   public:

   mwWindowManager mWM = mwWindowManager(10);
   mwRect<int> selection =  mwRect<int>::fromX1Y1WH(0,0,0,0);
   int mode;

   void init(int level);
   void init_windows();

   void set_mode(int new_mode);

   int loop(int level);

   int active = 1;

   int gx; // mouse position relative to scaled level background (0-99)
   int gy;

   int hx; // mouse position relative to scaled level background (0-1999)
   int hy;

   bool pos_valid;


   void get_mouse_position_on_background();
   void process_mouse();
   void process_keypress();
   void process_scrolledge();

   void show_selection_rect(mwRect<int> rect, int color, const char * text);
   void show_selection_text(mwRect<int> rect);

   bool get_new_box(const char* text, bool preview = false);

   void redraw_background(int mode);
   int redraw_background();

   void save_mW();
   void load_mW();
};

extern mwLevelEditor mLevelEditor;

#endif



