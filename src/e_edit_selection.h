// e_edit_selection.h

void es_pointer_text(int x1, int x2, int y, int mouse_on_window);
void es_do_brf(int x, int y, int flood_block);
void es_clear_ft(void);
int es_load_selection(void);
void es_save_selection(int save);
void es_do_fcopy(int qx1, int qy1);
void es_do_clear(void);
void set_block_with_flag_filters(int x, int y, int tn);
int es_draw_buttons(int x3, int x4, int yfb, int draw_only);
void es_draw_item_ft(int i);
void es_draw_enemy_ft(int e);
void es_draw_lifts_ft();
void es_draw_fsel(void);
void es_process_mouse(void);

