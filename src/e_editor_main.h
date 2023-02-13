// e_editor_main.h
void em_set_swbl(void);
void em_set_block_range(void);
char* em_get_text_description_of_block_based_on_flags(int flags);
void em_show_draw_item_cursor(void);
void em_show_item_info(int x, int y, int color, int type, int num);
void em_find_point_item(void);
void em_process_mouse(void);
int edit_menu(int el);
