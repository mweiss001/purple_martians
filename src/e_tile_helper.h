// e_tile_helper.h

extern int thl[100][100]; // tile helper

void th_replace(int type);
int th_draw_buttons(int x3, int x4, int yfb, int draw_only);
int th_compare_tile(int rb, int cb, int group);
void th_find_connected(int x, int y, int group);
void th_process_mouse(void);



