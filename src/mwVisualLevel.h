// mwVisualLevel.h

#define NUM_LEV 400

class mwVisualLevel
{
   public:
   int load_visual_level_select_done = 0;

   int visual_level_select();
   void level_viewer();

   int max_level_num;

   ALLEGRO_BITMAP* level_icon_vls[100];
   int level_icon_size;
   void create_level_icons_vls();
   void load_level_icons_vls();

   private:
   ALLEGRO_BITMAP* grid_bmp = NULL;
   ALLEGRO_BITMAP* level_icon_bmp[NUM_LEV];
   ALLEGRO_BITMAP* le_temp = NULL;
   int le[NUM_LEV]; // level exists array
   int num_levs;
   int sel_x, sel_y, sel_size;
   int grid_cols, grid_rows, grid_size, grid_width, grid_height;
   int sel = 0;
   int old_sel = 0;
   int cur = 0;
   int old_cur = 0;
   int draw_frames = 1;
   int vl_redraw = 1;

   void mark_rect(int sel, int color);
   void show_cur();
   void show_msel();
   void compare_curr(int sel);
   void compare_all();
   void lev_draw(int full);
   void show_cur_vs(int cur, int x1, int y1, int size, int fc);
   void load_visual_level_select();
};
extern mwVisualLevel mVisualLevel;


