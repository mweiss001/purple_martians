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






   int mouse_on_obj = 0;

   // vinepod only
   int mouse_on_vpodx = 0;
   int mouse_on_vpod1 = 0;
   int mouse_on_vpod2 = 0;
   int mouse_on_tb_ul = 0;
   int mouse_on_tb_lr = 0;

   // cloner only
   int mouse_on_csb_ul = 0;
   int mouse_on_csb_lr = 0;
   int mouse_on_cdb_ul = 0;

   // enemy shot prox
   int mouse_on_esp = 0;

   int mouse_on_sp = 0;
   int mouse_on_bmb = 0;
   int mouse_on_bms = 0;


   int mouse_on_item_sec_ul = 0;
   int mouse_on_item_sec_lr = 0;

   int mouse_on_lift = 0;

   int mouse_move = 0;
   int mouse_adj = 0;





















   int create_obj(int obt, int type, int num);
   void ov_get_size();
   void set_switch_tile(int i);
   void ov_title(int x1, int x2, int y1, int y2, int legend_highlight);


   void odbi(int d, int xa, int xb, int &ya, int bts, int col, int &var, int ul, int ll, int sl_inc, int st1_inc, int st2_inc, const char* txt);
   void odb0(int d, int xa, int xb, int &ya, int bts, int col, int &var, int ul, int ll, int sl_inc, int st1_inc, int st2_inc, const char* txt, const char* txt0);
   void odbf(int d, int xa, int xb, int &ya, int bts, int col, float &var, float ul, float ll, float sl_inc, float st1_inc, float st2_inc, const char* txt);
   void odbt(int d, int xa, int xb, int &ya, int bts, int colt, int &var, int col0, int col1,  const char* txt0, const char* txt1);

   bool odbb(int d, int xa, int xb, int &ya, int bts, int col, const char* txt);




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