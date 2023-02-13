// e_object_viewer.h

int create_obj(int obt, int type, int num);
void ov_get_size(void);
void set_switch_tile(int i);
void ov_title(int x1, int x2, int y1, int y2, int legend_highlight);
void ov_draw_buttons(int x1, int y1, int x2, int y2, int draw_only);
void ov_draw_overlays(int legend_highlight);
void ov_process_mouse(void);
void ov_set_to_0(void);
void ov_check_if_valid(int type);
void ov_process_keypress(void);
void object_viewer(int obt, int num);

