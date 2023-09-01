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

   void load_settings(void);
   void save_settings(void);


   void redraw_one_fcontrol(int x, int y, int &ya, int bts, int tc, int show_buttons, int num, int num_lines, int i, int &key, const char* nam);
   int redraw_all_fcontrols(int x, int y, int bts, int tc, int show_buttons, int num, int num_lines);

   void redraw_one_control(int x, int y, int &ya, int bts, int tc, int show_buttons, int num, int i, int &key, const char* nam);
   int redraw_all_controls(int x, int y, int bts, int tc, int show_buttons, int num);

   private:
   void set_all_logging(int v);
   void cfp_draw_player(int pco, int x, int y);
   int cfp_draw_line(int xa, int xb, int ya, int line_spacing, int col);
   void cfp_4tog(int xa, int xb, int &ya, int bts, int tc, int fc, int line_spacing, int index, const char * name);
   void draw_tab(struct settings_tab st[], int p, int col, int text_color);
};
extern mwSettings mSettings;

