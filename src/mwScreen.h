// mwScreen.h

class mwScreen
{
   public:


   // z_screen.h
   void get_new_background(int full);

   void transition_cutscene(int i, int f);

   void do_transition(float fmxi, float fmyi, float fmxf, float fmyf, float sci, float scf, float num_steps, float delay);

   int gate_transition_x;
   int gate_transition_y;
   int gate_transition_wx;
   int gate_transition_wy;
   float gate_transition_scale;


   int transition_num_steps;
   int transition_delay;


   void draw_hyst_rect(void);

   void set_screen_display_variables(void);

   void set_level_display_region_xy(void);



   void draw_scaled_level_region_to_display(int type);
   void set_map_var(void);
   void mark_non_default_block(int x, int y);

   void init_level_background(void);

   void draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players);

   void draw_level_centered_on_player_pos(int screen_x, int screen_y, float scale_factor);

   void draw_level(void);


   void draw_screen_frame(void);


   void frame_and_title(int show_players);
   void rtextout_centre(ALLEGRO_FONT *f, ALLEGRO_BITMAP *dbmp, const char *txt1, int x, int y, int col, float scale, float op);
   void draw_title(int tx, int ty, int ttw, int tth, int color);
   void draw_large_text_overlay(int type, int color);
   void draw_percent_barc(int cx, int y, int width, int height, int percent, int c1, int c2, int fc);
   void draw_percent_bar(int cx, int y, int width, int height, int percent);
   void draw_percent_bar_line(int cx, int y, int width, int height, int rise, int color, int percent);
   void draw_percent_bar_range(int cx, int y, int width, int height, int color, int start, int end);
   void draw_percent_barf(float x1, float x2, float y1, float y2, float percent);

   // z_screen_overlay.h
   int dif_from_now_to_nl();
   void show_player_stat_box(int tx, int y, int p);
   void show_level_done(void);
   void draw_screen_overlay(void);
   void set_player_join_quit_display(int p, int type, int time);
   void show_player_join_quit(void);
   void sdg_show_column(int col, int &x, int y);
   void sdg_show(int x, int y);
   void cdg_show(int x, int y);
   void draw_top_frame(int p);
   void draw_bottom_frame(int p);
   void draw_common_debug_overlay(int p, int &cx, int &cy);
   void draw_server_debug_overlay(int p, int &cx, int &cy);
   void draw_client_debug_overlay(int p, int &cx, int &cy);
   void draw_demo_debug_overlay(int p, int &cx, int &cy);


};
extern mwScreen mScreen;


