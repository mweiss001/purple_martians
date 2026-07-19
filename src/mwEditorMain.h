#ifndef PM_MWEDITORMAIN
#define PM_MWEDITORMAIN
#include "mwRect.h"

class mwEditorMain
{
public:
   mwEditorMain();

   void init();

   char* get_text_description_of_block_based_on_flags(int flags, char * msg);
   void show_draw_item_cursor();
   void show_item_info(int x, int y, int color, int type, int num);
   void find_point_item();
   void process_mouse();

   int draw_tile_mode = 1;
   int draw_item_num;
   int draw_item_type;
   int point_item_type;
   int point_item_num;
   int show_flag_details;
   int show_non_default_blocks;

   int show_tile_overlays;

   int status_window_has_mouse;
   int level_editor_quit_confirmation_dialog = 1;


   void draw_status_window(mwRect<int> rect, int draw_only, int have_focus);

   void cm_process_menu_bar(int d);


   int collapsed;
   int filter_mode;
   int obj_filter[5][20];
   void draw_filter_window(mwRect<int> &rect, int d);


   void draw_level_editor_background_overlays(int mouse_on_window);





};

extern mwEditorMain mEditorMain;

#endif
