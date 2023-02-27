// mwSettings.h

struct settings_tab
{
   char title[80];
   float x1;
   float y1;
   float x2;
   float y2;
};

class mwSettings
{
   public:
   void settings_pages(int set_page);

   int settings_current_page = 0;
   int overlay_grid[10][4] = {0};
   int number_of_debug_overlay_modes = 2;
   int redraw_all_controls(int x, int y, int bts, int tc, int show_buttons, int num);

   private:
   void set_all_logging(int v);
   void cfp_draw_player(int pco, int x, int y);
   int cfp_draw_line(int xa, int xb, int ya, int line_spacing, int col);
   void cfp_4tog(int xa, int xb, int &ya, int bts, int tc, int fc, int line_spacing, int index, const char * name);
   void draw_tab(struct settings_tab st[], int p, int col, int text_color);
};
extern mwSettings mSettings;

