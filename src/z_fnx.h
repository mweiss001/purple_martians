// z_fnx.h

int mw_draw_line2(double x1, double y1, double x2, double y2, float thickness, int c0_val, int c0_col, int c1_val, int c1_col, int &line_color_offset);
int mw_draw_line3(double x1, double y1, double x2, double y2, float thickness, int c0_val, int c0_col, int c1_val, int c1_col, int c2_val, int c2_col, int &line_color_offset);
void set_start_level(int s);
void set_player_color(int p, int c);
void set_speed(void);
int round20(int val);
void spin_shape(int tn, int x, int y, int tsx, int tsy, int tsw, int tsh, float scale, float dim, int cycle);
void change_block(int x, int y, int block);
void get_hostname(int print);
void mw_get_text_dimensions(ALLEGRO_FONT *f, const char* txt, int &bx, int &by, int &bw, int &bh);
void make_palette(void);
void m_err(const char * err_msg);
void window_title(void);
int is_block_empty(int x, int y, int test_block, int test_item, int test_enemy);
void tsw(void);



void set_enemy_rot_from_incs(int e);
void set_enemy_rot_from_player(int e, int p);
void seek_set_xyinc(int e);
int find_closest_player(int e);
int find_closest_player_flapper(int e);
int find_closest_player_trakbot(int e);
int find_closest_player_cannon(int e);
float deg_to_rad(float deg);



void show_var_sizes(void);
