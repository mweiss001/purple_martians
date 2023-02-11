// mwWindow.h
#ifndef mwWindow_H
#define mwWindow_H

class mwWindow
{
   public:
   int x1, y1, x2, y2, w, h;
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

   // mW[1] - status window only
   int draw_item_num;
   int draw_item_type;
   int point_item_type;
   int point_item_num;
   int show_flag_details;
   int show_non_default_blocks;

   // mW[2] - select window only
   int select_window_block_on;
   int select_window_special_on;
   int select_window_num_special_lines;
   int swbn;  // number of blocks
   int swnbl; // number of block lines (just swbn / 16 + 1)
   int swnbl_cur; // current number of lines shown

   // mW[3] - filter window only
   int collapsed;
   int filter_mode;

   // mW[4] - selection edit window only
   int copy_mode;
   int brf_mode;
   int sw;
   int sh;

   // mW[5] - group edit list only
   int show_sel_frame;

   // mW[6] - group edit controls only

   // mW[7] - object viewer window only
   int obt;
   int num;
   int mb;
   int viewer_lock;
   int legend_line;
   int num_legend_lines;
   int pop_msg_viewer_pos;

   // mW[8] - top menu only

   // mW[9] - tile helper only
   int th_add_del;
   int th_match;
   int th_group;
   int th_sel;

   mwWindow(); // default constructor
   void set_pos(int, int);
   void set_size(int, int);
   void draw(int draw_only);
   int detect_mouse(void);
   void set_title(const char*);
   void process_mouse(void);
   void check_offscreen(void);
};
#endif
