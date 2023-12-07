// mwScreen.h

class mwScreen
{
   public:


   // the viewport region of the level
   float level_display_region_x;
   float level_display_region_y;
   float level_display_region_xinc;
   float level_display_region_yinc;
   int level_display_region_w;
   int level_display_region_h;

   int ldr_xmx_h;
   int ldr_xmn_h;
   int ldr_ymx_h;
   int ldr_ymn_h;

   int ldr_xmx_a;
   int ldr_xmn_a;
   int ldr_ymx_a;
   int ldr_ymn_a;

   int screen_display_x;
   int screen_display_y;
   int screen_display_w;
   int screen_display_h;



   int viewport_mode;
   int viewport_show_hyst;
   float viewport_x_div;
   float viewport_y_div;

   int viewport_look_up_down;
   int viewport_look_player_facing_left_right;

   int viewport_look_player_motion;
   int viewport_look_rocket;




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


   // the position and size of the menu
   int menu_x;
   int menu_y;
   int menu_w;
   int menu_h;


   // the position of the level info text
   int menu_level_info_x;
   int menu_level_info_y;
   int menu_level_info_w;


   // the size and position of the level display under the menu
   int menu_level_display_size;
   int menu_level_display_x;
   int menu_level_display_y;

   // the scale and position of the spinning logo beside the menu
   float menu_logo_scale;
   float menu_logo_x;
   float menu_logo_y;

   // the scale and position of the spinning logo in full screen splash mode
   float splash_logo_scale;
   float splash_logo_x;
   float splash_logo_y;

   // transition from splash logo to menu logo
   float splash_logo_scale_dec;
   float splash_logo_x_dec;
   float splash_logo_y_dec;




   void draw_hyst_rect(void);

   void set_screen_display_variables(void);

   void set_level_display_region_xy(void);










   void draw_scaled_level_region_to_display(int type);
   void set_map_var(void);
   void mark_non_default_block(int x, int y);

   void init_level_background(void);

   void draw_level2(ALLEGRO_BITMAP *b, int mx, int my, int ms, int blocks, int items, int enemies, int lifts, int players);

   void draw_level_centered_on_player_pos(int screen_x, int screen_y, float scale_factor);

   void draw_level_map_under_menu(void);


   void draw_screen_frame(void);

   void draw_line_of_players(void);

   void draw_level_info(void);

   void frame_and_title(void);

   void rtextout_centref(ALLEGRO_FONT *f, ALLEGRO_BITMAP *dbmp, int x, int y, int col, float scale, float op, const char *format, ...);
   void rtextout_centre(ALLEGRO_FONT *f, ALLEGRO_BITMAP *dbmp, int x, int y, int col, float scale, float op, const char *txt);

   void draw_framed_text(int xc, int y, int round, ALLEGRO_FONT *f, int fc, int tc, const char* txt);

   void draw_title(int tx, int ty, int ttw, int tth, int color);
   void draw_large_text_overlay(int type, int color);
   void draw_percent_barc(int cx, int y, int width, int height, int percent, int c1, int c2, int fc);
   void draw_percent_bar(int cx, int y, int width, int height, int percent);
   void draw_percent_bar_line(int cx, int y, int width, int height, int rise, int color, int percent);
   void draw_percent_bar_range(int cx, int y, int width, int height, int color, int start, int end);
   void draw_percent_barf(float x1, float x2, float y1, float y2, float percent);




   // screen overlay

   void set_player_text_overlay(int p, int type);
   void show_player_text_overlay(void);
   int player_text_overlay_timer;
   int player_text_overlay_type;
   int player_text_overlay_player;
   int player_text_overlay_timer_reset_val = 80;

   int dif_from_now_to_nl();
   void show_player_stat_box(int tx, int y, int p);
   void show_level_done(void);

   void calc_actual_screen_position(float ex, float ey, float &ex1, float &ey1);

   void draw_screen_overlay(void);


   ALLEGRO_COLOR col_clr(int p);
   ALLEGRO_COLOR col_clr2(int p);


   void drg_show(int x, int y); // demo record debug grid

   void sdg_show_column(int col, int &x, int y);
   void sdg_show(int x, int y);
   void cdg_show(int x, int y);
   void draw_top_frame(int p);
   void draw_bottom_frame(int p);
   void draw_common_debug_overlay(int p, int &cx, int &cy);

   void draw_viewport_debug_overlay(int p, int &cx, int &cy);


   void draw_bandwidth_stats(int &cx, int &cy);



   void draw_server_debug_overlay(int &cx, int &cy);
   void draw_client_debug_overlay(int &cx, int &cy);
   void draw_demo_debug_overlay(int p, int &cx, int &cy);


};
extern mwScreen mScreen;


