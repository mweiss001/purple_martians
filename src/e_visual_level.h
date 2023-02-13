// e_visual_level.h
int lev_show_level_data(int x_pos, int y_pos);
void mark_rect(int sel, int color);
void show_cur(void);
void show_msel(void);
void compare_curr(int sel);
void compare_all(void);
void lev_draw(int full);
void level_viewer(void);
void show_cur_vs(int cur, int x1, int y1, int size, int fc);
void load_visual_level_select(void);
int visual_level_select(void);

#define NUM_LEV 400
extern ALLEGRO_BITMAP * grid_bmp;
extern ALLEGRO_BITMAP * level_icon_bmp[NUM_LEV];
extern int le[NUM_LEV];
extern int num_levs;
extern int sel_x, sel_y, sel_size;
extern int grid_cols, grid_rows, grid_size, grid_width, grid_height;
extern int load_visual_level_select_done;
