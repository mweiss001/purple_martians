#ifndef PM_MWOBJECTVIEWER_H
#define PM_MWOBJECTVIEWER_H
#include "mwRect.h"

class mwObjectViewer
{

public:

   void init();

   mwObjectViewer() { init(); }


   int obt;
   int num;
   int mb;
   int viewer_lock;
   int legend_line;
   int num_legend_lines;
   int pop_msg_viewer_pos;
   int snap;

   int create_obj(int obt, int type, int num);
   void ov_get_size();
   void set_switch_tile(int i);
   void ov_title(int x1, int x2, int y1, int y2, int legend_highlight);
   void ov_draw_buttons(int x1, int y1, int x2, int y2, int draw_only);

   void ov_draw_overlay_rectangle_and_crosshairs(int x1, int y1, int w, int h, int color, int crosshairs);

   void ov_draw_overlays(int legend_highlight);

   void ov_move_enemy(int num, int x_offset, int y_offset);
   void ov_move_item(int num, int x_offset, int y_offset);

   void ov_b3_arrow_nudge(int& x, int& y);

   void ov_process_mouse_on_background();
   void ov_check_if_valid();
   void ov_process_keypress();
   void object_viewer(int obt, int num);

   void draw(mwRect<int> &rect, int draw_only, int have_focus);

   void draw_level_editor_background_overlays(int mouse_on_window);

};

extern mwObjectViewer mObjectViewer;


#endif