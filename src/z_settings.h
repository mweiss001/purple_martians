// z_settings.h

extern int settings_current_page;

extern int overlay_grid[10][4];



//void set_all_logging(int v);
int redraw_all_controls(int x, int y, int bts, int tc, int show_buttons, int num);
//void cfp_draw_player(int pco, int x, int y);
//int cfp_draw_line(int xa, int xb, int ya, int line_spacing, int col);
//void draw_tab(struct settings_tab st[], int p, int col, int text_color);
void settings_pages(int set_page);


