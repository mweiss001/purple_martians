// mwSettings.h

struct settings_tab
{
   char title[80];
   int show;
   float x1;
   float y1;
   float x2;
   float y2;
};

class mwSettings
{
   public:
   void settings_pages(int set_page);

   int current_page = 0;
   int show_advanced;
   int show_debug;

   int overlay_grid[10][4] = {0};
   int number_of_debug_overlay_modes = 2;

   void load_settings();
   void save_settings();


   void reset_overlay_settings();


   int log_group = 1;

   void redraw_one_fcontrol(int x, int y, int &ya, int bts, int tc, int show_buttons, int num, int num_lines, int i, int &key, const char* nam);
   int redraw_all_fcontrols(int x, int y, int bts, int tc, int show_buttons, int num, int num_lines);

   void redraw_one_control(int x, int y, int &ya, int bts, int tc, int show_buttons, int num, int i, int &key, const char* nam);
   int redraw_all_controls(int x, int y, int bts, int tc, int show_buttons, int num);

   private:


   int fc = 10; // frame color
   int tc = 15; // text color



   int cf_w;
   int cf_x1;
   int cf_x2;

   int cfp_x1;
   int cfp_x2;
   int cfp_txc;


   int cf_h;
   int cf_y1;
   int cf_y2;

   int cfp_y1;



   void set_all_logging(int v);
   void cfp_draw_player(int pco, int x, int y);
   int cfp_draw_line(int xa, int xb, int ya, int line_spacing, int col);
   void cfp_4tog(int xa, int xb, int &ya, int bts, int tc, int fc, int line_spacing, int index, const char * name);
   void draw_tab(struct settings_tab st[], int p, int col, int text_color);


   void page_main();
   void page_mode();
   void page_controls();
   void page_controls2();
   void page_netgame();
   int page_demo();
   void page_level_bottom_msg();
   void page_level_stats();
   void page_transitions();
   void page_viewport();
   void page_overlay();
   void page_double();
   void page_speed();
   void page_info();
   int page_log();
   void page_misc();


};
extern mwSettings mSettings;

