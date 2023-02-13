// z_screen_overlay.h

int dif_from_now_to_nl();
void show_player_stat_box(int tx, int y, int p);
void show_level_done(void);
void draw_screen_overlay(void);
void set_player_join_quit_display(int p, int type, int time);
void show_player_join_quit(void);
void draw_top_frame(int p);
void draw_bottom_frame(int p);
void draw_common_debug_overlay(int p, int &cx, int &cy);
void draw_server_debug_overlay(int p, int &cx, int &cy);
void draw_client_debug_overlay(int p, int &cx, int &cy);
void draw_demo_debug_overlay(int p, int &cx, int &cy);
void game_event(int ev, int x, int y, int z1, int z2, int z3, int z4);
