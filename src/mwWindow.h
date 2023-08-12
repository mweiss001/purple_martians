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
   void cm_draw_status_window(int x1, int x2, int y1, int y2, int draw_only, int have_focus);


  // e_editor_main.h
   void em_set_swbl(void);
   void em_set_block_range(void);
   char* em_get_text_description_of_block_based_on_flags(int flags, char*);
   void em_show_draw_item_cursor(void);
   void em_show_item_info(int x, int y, int color, int type, int num);
   void em_find_point_item(void);
   void em_process_mouse(void);

   // mW[2] - select window only
   int select_window_block_on;
   int select_window_special_on;
   int select_window_num_special_lines;
   int swbn;  // number of blocks
   int swnbl; // number of block lines (just swbn / 16 + 1)
   int swnbl_cur; // current number of lines shown
   void cm_draw_selection_window(int x1, int x2, int y1, int y2, int draw_only, int have_focus);


   // mW[3] - filter window only
   int collapsed;
   int filter_mode;
   int cm_draw_filter_buttons(int x1, int x2, int y1, int mode, int draw_only);


   // mW[4] - edit selection window only
   int copy_mode;
   int brf_mode;
   int sw;
   int sh;

   // e_edit_selection.h
   void es_pointer_text(int x1, int x2, int y, int mouse_on_window);
   void es_do_brf(int x, int y, int flood_block);
   void es_clear_ft(void);
   int es_load_selection(void);
   void es_selection_to_ft(int save_to_disk);
   void es_do_fcopy(int qx1, int qy1);
   void es_do_clear(void);
   void set_block_with_flag_filters(int x, int y, int tn);
   int es_draw_buttons(int x3, int x4, int yfb, int draw_only);
   void es_draw_item_ft(int i);
   void es_draw_enemy_ft(int e);
   void es_draw_lifts_ft();
   void es_draw_fsel(void);
   void es_process_mouse(void);


   // mW[5] - group edit list only
   int show_sel_frame;

   // e_group_edit.h

   void ge_init_data(void);
   void ge_set_valid_controls(void);

   void ge_collapse_all_controls(void);
   void ge_expand_all_controls(void);
   void ge_clear_obj_list(void);

   void ge_add_to_obj_list(int t, int i);
   void ge_remove_obj_list_item(int o);
   void ge_remove_obj_list_filtered_items(void);
   void ge_swap_obj_list_items(int i1, int i2);

   void ge_object_initial_position_random(int typ, int num);

   int ge_draw_list_items(int x1, int y1, int ni);
   void ge_show_obj_list(int x, int y, int *ew, int *eh, int draw_only);

   void ge_set_var(int gvt, int gvn, int num, float val);
   float ge_get_var(int gvt, int gvn, int num);

   int ge_show_controls(int x, int y, int *ew, int *eh, int hidden, int draw_only);
   void ge_add_selection_to_list(int set_filters);
   void ge_process_mouse(void);

   // mW[6] - group edit controls only

   // mW[7] - object viewer window only
   int obt;
   int num;
   int mb;
   int viewer_lock;
   int legend_line;
   int num_legend_lines;
   int pop_msg_viewer_pos;

   int snap;


   // e_object_viewer.h
   int create_obj(int obt, int type, int num);
   void ov_get_size(void);
   void set_switch_tile(int i);
   void ov_title(int x1, int x2, int y1, int y2, int legend_highlight);
   void ov_draw_buttons(int x1, int y1, int x2, int y2, int draw_only);

   void ov_draw_overlay_rectangle_and_crosshairs(int x1, int y1, int w, int h, int color, int crosshairs);

   void ov_draw_overlays(int legend_highlight);

   void ov_move_enemy(int num, int x_offset, int y_offset);
   void ov_move_item(int num, int x_offset, int y_offset);

   void ov_b3_arrow_nudge(int& x, int& y);

   void ov_process_mouse(void);
   void ov_check_if_valid(void);
   void ov_process_keypress(void);
   void object_viewer(int obt, int num);

   // mW[8] - top menu only
   void cm_process_menu_bar(int draw_only);

   // mW[9] - tile helper only
   int th_add_del;
   int th_match;
   int th_group;
   int th_sel;
   void th_replace(int type);
   int th_draw_buttons(int x3, int x4, int yfb, int draw_only);
   int th_compare_tile(int rb, int cb, int group);
   void th_find_connected(int x, int y, int group);
   void th_process_mouse(void);



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
