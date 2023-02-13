// z_menu.h

extern char global_string[20][25][80];
extern char msg[1024];


int load_help(void);
void chop_first_x_char(char *str, int n);
void help(const char *topic);
int tmenu(int menu_num, int menu_pos, int x, int y);
int zmenu(int menu_num, int menu_pos, int y);
int pmenu(int menu_num, int bg_color);
void menu_setup(void);
void show_cursor(char *f, int cursor_pos, int xpos_c, int ypos, int cursor_color, int restore);
int edit_pmsg_text(int c, int new_msg);
void edit_server_name(int type, int x, int y);
int edit_lift_name(int lift, int step_ty, int bts, char *fst);
