// mwVisualLevel.h

#define NUM_LEV 400

class mwVisualLevel
{


// visual level select
// --------------------
public:
   int load_visual_level_select_done = 0;
   int visual_level_select(int max_level);
private:
   ALLEGRO_BITMAP* grid_bmp = NULL;
   void load_visual_level_select(int max_level, int &sel_x, int &sel_y, int &sel_size, int &grid_cols, int &grid_rows, int &grid_size, int &grid_width, int &grid_height);
   void show_cur_vs(int cur, int x1, int y1, int size, int fc);
   int level_exists_array[NUM_LEV];
   int num_levs_found;



// level viewer
// -------------
public:
   void level_viewer();
private:
   ALLEGRO_BITMAP* le_temp = NULL;
   void lev_draw(int full, int sel, int cur);
   void show_big_lev(int level, const char * text, int x, int y, int color);
   void mark_rect(int sel, int color);
   void compare_curr(int sel);
   void compare_all();

};
extern mwVisualLevel mVisualLevel;


/*
   visual level select is used in 3 places

- game menu
- level editor menu
- remote control to select new level

it now uses the icons from mwLevelIcon

this is the way it is called:
int visual_level_select(int max_level);

remote and game only do 100, but level editor does 400

every time it is called it recreates the background grid tilemap
when it is running if the screen size changes it is also rebuilt





















































*/