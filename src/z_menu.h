// z_menu.h

extern char msg[1024];

void show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore);
void edit_server_name(int type, int x, int y);
int edit_lift_name(int lift, int step_ty, int bts, char *fst);
