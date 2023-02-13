// e_group_edit.h

void ge_init_data(void);
void ge_set_valid_controls(void);
void ge_add_to_obj_list(int t, int i);
void ge_remove_obj_list_item(int o);
void ge_remove_obj_list_filtered_items(void);
void ge_swap_obj_list_items(int i1, int i2);
void ge_enemy_initial_position_random(int e, int csw);
void ge_item_initial_position_random(int i, int csw);
int ge_draw_list_items(int x1, int y1, int ni);
void ge_show_obj_list(int x, int y, int *ew, int *eh, int draw_only);
int ge_show_controls(int x, int y, int *ew, int *eh, int hidden, int draw_only);
void ge_add_selection_to_list(int set_filters);
void ge_process_mouse(void);
