// z_screen.h

void get_new_background(int full);
void stimp(void);
void stamp(void);
void draw_hyst_rect(void);
void get_new_screen_buffer(int type, int x, int y);
void set_map_var(void);
void mark_non_default_block(int x, int y);
void init_level_background(int);
void draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players);
void draw_level_centered(int screen_x, int screen_y, int level_x, int level_y, float scale_factor);
void draw_level(void);
int get_contrasting_color(int color);
void frame_and_title(int show_players);
void rtextout_centre(ALLEGRO_FONT *f, ALLEGRO_BITMAP *dbmp, const char *txt1, int x, int y, int col, float scale, int rot, float op);
void draw_title(int tx, int ty, int ttw, int tth, int color);
void draw_large_text_overlay(int type, int color);
void draw_percent_barc(int cx, int y, int width, int height, int percent, int c1, int c2, int fc);
void draw_percent_bar(int cx, int y, int width, int height, int percent);
void draw_percent_bar_line(int cx, int y, int width, int height, int rise, int color, int percent);
void draw_percent_bar_range(int cx, int y, int width, int height, int color, int start, int end);
